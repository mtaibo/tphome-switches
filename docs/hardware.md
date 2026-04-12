# Hardware

This document covers the physical pin mappings, electrical logic and hardware-specific behaviour for each supported device. Review this carefully before flashing onto any board.

---

## Matismo WIP100 — TYWE3S (ESP8266)

The TYWE3S module houses an ESP8266 running at 80MHz with 1MB of flash. It is the primary supported platform and the only one currently in stable status.

### Pin Mapping

| GPIO | Function | Notes |
|---|---|---|
| 0 | `LED_GREEN` | Network status indicator |
| 1 | `LED_CONFIG` | **Shared with TX** — drives the blue backlight common cathode |
| 2 | `LED_BTM` | Dual-color LED (blue / red) |
| 3 | `BTN_MID` | **Shared with RX** — middle button |
| 5 | `BTN_BTM` | Bottom button |
| 12 | `BTN_TOP` | Top button |
| 13 | `RELAY_UP` | Motor relay — output L1 |
| 14 | `LED_MID` | Dual-color LED (blue / red) |
| 15 | `RELAY_DOWN` | Motor relay — output L3 |
| 16 | `LED_TOP` | Dual-color LED (blue / red) |

> **GPIO 1 / GPIO 3 (TX/RX):** These pins are shared with the serial interface. GPIO 1 (`LED_CONFIG`) is used as a digital output to drive the blue LED common cathode — pulling it `HIGH` enables the blue channel on all three dual-color LEDs, pulling it `LOW` or setting it to `INPUT` disables them. GPIO 3 (`BTN_MID`) is read as a button. If you need the serial monitor for debugging, be aware that both pins will have interference from the UART.

> **GPIO 6–11:** Reserved for internal SPI flash. Do not use.

---

### LED Logic

There are two distinct LED types on this board.

**Green LED (GPIO 0) — single color**

| State | Meaning |
|---|---|
| `HIGH` (on) | Active — used for network feedback |
| `LOW` (off) | Inactive |

**Dual-color LEDs — Top / Mid / Bottom (GPIO 16 / 14 / 2)**

These are dual-color LEDs sharing a common cathode line driven by `LED_CONFIG` (GPIO 1). Their color depends on the output level of each individual pin:

| Output | Color | Meaning |
|---|---|---|
| `LOW` | Blue | Idle / OK |
| `HIGH` | Red | Busy / Error / Active |

In code, `Leds::BLUE` is an alias for `Leds::OFF` and `Leds::RED` is an alias for `Leds::ON` — the naming reflects the visual result rather than the electrical level.

`Hardware::enableLight()` sets GPIO 1 to `OUTPUT` and `HIGH` to activate the blue channel. `Hardware::disableLight()` pulls it `LOW`. Both are called by `Leds::off()` and during mode transitions.

---

### Button Logic

All buttons are wired with internal pull-up resistors (`INPUT_PULLUP`). Logic is active-low:

| State | Reading |
|---|---|
| Pressed | `LOW` (0) |
| Released | `HIGH` (1) |

Press duration is measured by `InputPin` in centiseconds. When a press exceeds `LONG_PULSE` (5s) the event fires immediately without waiting for release, to give instant feedback on long-press actions.

---

### Relay Logic

Relays control current flow to the blind motor. Only one relay should be active at a time — activating both simultaneously would short the motor. The firmware enforces this through `Relays::stop()`, which always cuts both relays before activating either direction.

| Relay | GPIO | Motor direction (default) |
|---|---|---|
| `RELAY_UP` | 13 | Ascent (L1) |
| `RELAY_DOWN` | 15 | Descent (L3) |

If the motor is physically wired in reverse, set `invertedRelays = true` in the device preferences (via `AdminCmd::SET_PREFS`). The firmware will swap which relay it activates for each direction without any code change.

After any direction change, the motor is held off for `MOTOR_SAFE_TIME` (1s) before the new direction starts. This protects against back-EMF and mechanical stress on direction reversal.

---

### LED Feedback Reference

The following table summarises the LED states the firmware uses across all modes and events.

| Situation | LED | State |
|---|---|---|
| WiFi connecting | Green | Fast blink |
| MQTT connecting | Green | Slow blink |
| Connected (normal mode) | Green | Off |
| Disconnected timeout (normal mode) | Green | On (solid) |
| CONNECTION mode active | Top | Slow blink |
| WiFi connected (connection mode) | Mid | Blue |
| WiFi disconnected (connection mode) | Mid | Red |
| MQTT connected (connection mode) | Btm | Blue |
| MQTT disconnected (connection mode) | Btm | Red |
| Blind moving up | Top | Red (on) |
| Blind moving down | Btm | Red (on) |
| Blind stopped | Mid | Blue (brief blink) |
| AP provisioning active | Green | On (solid) |
| OTA update in progress | All | Off |

---

## Matismo WIP100 — CB3S (BK7231N)

> Development in progress. Pin mapping and driver implementation pending.

---

## BSeed Melody M1 — T34 (BK7231N)

> Development in progress. Pin mapping and driver implementation pending.
