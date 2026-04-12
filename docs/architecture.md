# Architecture

The firmware is organized in four independent layers. Each layer has a single responsibility and only depends on layers below it. This makes it straightforward to port the firmware to a new chip: only the `hardware/` layer needs to change.

```
┌─────────────────────────────────────┐
│              logic/                 │  Device behavior (blinds, buttons, LEDs)
├─────────────────────────────────────┤
│             network/                │  WiFi, MQTT, OTA, AP provisioning
├─────────────────────────────────────┤
│             settings/               │  Configuration, persistence, device mode
├─────────────────────────────────────┤
│             hardware/               │  Pin drivers, chip-specific abstractions
└─────────────────────────────────────┘
```

---

## Entry Point

`src/main.cpp` is the Arduino entry point. It is intentionally minimal — it delegates everything to the modules:

```cpp
void setup() {
    Settings::setup();   // Load config from NVS flash
    Hardware::setup();   // Configure GPIO pins
    Network::setup();    // Start WiFi + MQTT, set callback
}

void loop() {
    Network::update();   // WiFi and MQTT state machine
    Actions::check();    // Translate button events to device actions
    Leds::update();      // Advance LED blink/duration timers
    Buttons::update();   // Poll hardware buttons, classify press duration
    Blinds::update();    // Advance motor position state machine (blind builds only)
}
```

The loop is fully non-blocking. No `delay()` calls exist in the main execution path — all time-based logic uses `millis()` comparisons so the loop runs as fast as possible.

---

## Conditional Compilation

The same codebase produces firmware for different hardware chips and device types through preprocessor flags defined in `platformio.ini`:

**Hardware target** — selects the correct low-level drivers:
- `DEVICE_HARDWARE_ESP8266`
- `DEVICE_HARDWARE_BK7231N`

**Device type** — selects the correct application logic:
- `DEVICE_TYPE_BLIND`
- `DEVICE_TYPE_LIGHT`

Every module that needs to branch on these flags uses `#if defined(...)` guards. For example, `main.cpp` includes `blinds.h` or `lights.h` depending on device type, and `hardware.h` includes the correct driver folder depending on the chip.

This means a single `pio run -e blind_esp8266` produces a blind-specific binary for ESP8266, while `pio run -e light_bk7231` produces a light-specific binary for BK7231N — all from the same source tree.

---

## Module Breakdown

### `hardware/`

Low-level drivers. All chip-specific code lives here. The rest of the firmware never touches Arduino or chip SDK functions directly.

| File | Responsibility |
|---|---|
| `pinout.h` | `constexpr` pin number definitions in the `Pins::` namespace |
| `driver_input.h` | `InputPin<pin, maxTime>` template — polls a button and measures press duration |
| `driver_output.h` | `OutputPin<pin>` template — `on()`, `off()`, `deactivate()` |
| `driver_wifi.h` | Wraps chip WiFi SDK — `setup()`, `begin()`, `disconnect()`, `isConnected()` |
| `hardware.h` | Instantiates all pins as named type aliases (`BtnTop`, `RelayUp`, `LedMid`...) and provides `setup()` |

`InputPin` and `OutputPin` are template classes, not objects. Pin numbers are compile-time constants, so there is no runtime overhead for GPIO abstraction.

`InputPin` measures press duration in centiseconds (`millis() / 10`) to fit a 16-bit counter. It detects press start, tracks duration, enforces a maximum, and returns the measured duration on release.

### `settings/`

Handles all persistent state through the `Settings::` namespace.

| File | Responsibility |
|---|---|
| `defaults.h` | Compile-time constants for all default values and buffer sizes |
| `credentials.example.h` | Template for WiFi/MQTT credentials — copy to `credentials.h` and fill in |
| `settings.h` | Defines `Config`, `Prefs` and `State` packed structs; declares `save()`, `load()`, `reset()` |
| `mode.h` | Manages the two runtime modes: `NORMAL` and `CONNECTION` |
| `settings.cpp` | NVS read/write implementation using the `Preferences` library |

Three packed structs are persisted independently to NVS flash under the keys `"c"`, `"p"` and `"s"`:

- **`Config`** — identity and network credentials (deviceID, WiFi SSID/pass, MQTT host/user/pass/port). Written rarely.
- **`Prefs`** — device preferences (blind travel times, default down position, relay orientation). Written on admin command.
- **`State`** — current device state (blind position). Written on every stop to survive power cycles.

On `save()`, each struct is compared against what is already stored before writing — this avoids unnecessary flash wear when nothing has changed.

On `load()`, if the size of any stored block does not match its expected struct size (e.g. after a firmware update changes a struct), `defaults()` is called to reinitialise everything cleanly.

**Device ID format:** on first boot, the ID is derived from the last 4 hex digits of the MAC address (e.g. `A1B2`). Once the device is assigned a full ID by the backend via MQTT, the format becomes `{type}{zone:02d}{device:02d}` — e.g. `B0102` for blind 02 in zone 01.

### `network/`

Manages all connectivity. WiFi and MQTT each have independent state machines that operate without blocking the loop.

| File | Responsibility |
|---|---|
| `wifi.h` | WiFi connection state machine — up to 5 attempts, 5s interval, no auto-reconnect after failure |
| `mqtt.h` | MQTT connection state machine — topic construction, LWT, subscribe on connect |
| `commands.h` | MQTT callback, command parsing, state publishing |
| `network.h` | Orchestrates WiFi + MQTT, manages LED feedback per mode |
| `ap.h` | Access Point provisioning — serves a web form to configure credentials on first boot |
| `ota.h` | OTA update — fetches binary from `http://{mqttIP}/firmware/{deviceID}.bin` |

Both WiFi and MQTT follow the same pattern: try up to `MAX_ATTEMPTS` times with `ATTEMPT_INTERVAL` between each. After exhausting attempts, they stop trying and wait for a manual reconnect trigger (long press or MQTT admin command).

On MQTT connect, the client subscribes to different topics depending on whether the device has a full ID or is still unconfigured (4-char MAC-based ID). See [network.md](network.md) for the full topic structure.

The LWT (Last Will and Testament) publishes a single `0xFF` byte to the state topic when the device disconnects unexpectedly, allowing the backend to detect offline devices.

### `logic/`

Application-level behavior. This layer consumes hardware drivers and network modules but contains no chip-specific code.

| File | Responsibility |
|---|---|
| `buttons.h` | Reads `InputPin` durations, classifies into `SHORT` / `MEDIUM` / `LONG`, stores per-button pending action |
| `leds.h` | LED state manager — supports `ON`, `OFF`, `BLINK` with configurable speed, blink count and duration |
| `actions.h` | Maps button events to device actions, branching on current mode (`NORMAL` / `CONNECTION`) |
| `blinds.h` | Motor state machine and time-based position tracking |
| `lights.h` | Light control logic (in development) |

**Button classification** thresholds (defined in `defaults.h`):
- `SHORT` — any press ≥ 50ms and < `SHORT_PULSE` (2s)
- `MEDIUM` — press ≥ `SHORT_PULSE` (2s) and < `LONG_PULSE` (5s)
- `LONG` — press ≥ `LONG_PULSE` (5s)

**LED manager** maintains a fixed array of 4 slots. Each slot tracks pin, mode, blink interval, remaining blink count and expiry time. `Leds::set()` finds the slot for a given pin (or claims an empty one) and configures it. `Leds::update()` is called every loop iteration to advance timers and toggle state.

**Blind motor state machine** has four states:

```
IDLE ──► MOVING ──► STOPPING ──► IDLE
           │
           ▼
        WAITING ──► MOVING
```

- `IDLE` — motor off, position stable.
- `MOVING` — relays active, position updated every loop tick via `dt`-based calculation.
- `WAITING` — a direction change was requested while moving; relays are off for `MOTOR_SAFE_TIME` (1s) before the new direction starts.
- `STOPPING` — the blind reached 0% or 100% and continues running for `STOPPING_TIME` (2s) to ensure it is fully seated, then transitions to `IDLE`.

Position is tracked as a `uint16_t` in the range `0–10000` (representing 0.00%–100.00%). The internal calculation uses a `float` to accumulate fractional movement per tick, only casting back to `uint16_t` for storage.

---

## Design Notes

**Header-only modules.** With the exception of `settings.cpp`, the entire firmware is implemented in `.h` files using `inline` and `static` functions within namespaces. This avoids linker complexity while keeping logical separation between modules.

**No dynamic allocation.** No `new`, no `malloc`, no `std::vector`. All state lives in statically allocated structs and fixed-size arrays. This is intentional on memory-constrained chips like ESP8266 (80KB RAM).

**Packed structs for NVS and MQTT payloads.** `__attribute__((__packed__))` is used on all structs that are serialised — either written to flash or sent over MQTT. This ensures layout is predictable regardless of compiler padding.

**Time in centiseconds.** Several modules use `millis() / 10` cast to `uint16_t` for timing. This gives ~655 seconds of range in a 16-bit value, which is sufficient for LED blink intervals and button press durations while avoiding 32-bit arithmetic overhead in tight loops.
