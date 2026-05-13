<div align="center">

<img src="docs/banner.svg" alt="TPHome" width="100%" />

<br/>
<br/>

**The lowest layer of the stack — running on the chips inside every switch.**

<br/>

![Status](https://img.shields.io/badge/status-active-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)
![Made with](https://img.shields.io/badge/made%20with-C%2B%2B%20%7C%20PlatformIO%20%7C%20Arduino-lightgrey)

</div>

---

I'm a first-year computer engineering student and this repository is the lowest layer of a home automation system I built from scratch — the C++ firmware that runs directly on the chips inside commercial smart switches and blind controllers, replacing their factory software.

The hardware I target are off-the-shelf Wi-Fi connected devices (mostly Tuya-based) that ship with vendor-cloud-dependent firmware. I desolder the original chip, dump the factory flash, and flash my own code over a USB-TTL adapter. After that, the device lives entirely on my local network and answers to MQTT commands from the backend.

> **Note:** This is a personal project tailored to my specific hardware. Pin mappings must be reviewed before deploying on different boards.

## Repositories

<table>
<tr>
<td width="33%">

### tphome-firmware (this repo)

Custom C++ firmware for proprietary chips, built with PlatformIO.

Replaces factory software on commercial blind controllers and light switches with a fully local MQTT-based control layer.

**C++ · PlatformIO · Arduino**

</td>
<td width="33%">

### [tphome-api](https://github.com/mtaibo/tphome-api)

FastAPI backend running on a Raspberry Pi alongside a Mosquitto MQTT broker, managed with Docker.

Handles device management, state persistence, OTA firmware serving and real-time WebSocket events.

**Python · FastAPI · Docker · SQLite**

</td>
<td width="33%">

### [tphome](https://github.com/mtaibo/tphome)

Vue 3 frontend served by Nginx behind a Caddy reverse proxy, also hosted on the Raspberry Pi.

Renders an interactive SVG floor plan where you can see and control every light and blind in real time.

**Vue 3 · Tailwind CSS · Nginx**

</td>
</tr>
</table>

## Firmware architecture

The code is organised in four layers, where each layer only depends on the ones below it. Porting to a new chip means rewriting only the hardware layer.

```
┌─────────────────────────────────────────────┐
│                  logic/                     │
│  buttons · actions · blinds · leds · lights │
├─────────────────────────────────────────────┤
│                  network/                   │
│  mqtt · commands · wifi · ota · ap          │
├─────────────────────────────────────────────┤
│                  settings/                  │
│  defaults · mode · credentials              │
├─────────────────────────────────────────────┤
│                  hardware/                  │
│  esp8266/         bk7231n/ (in development) │
│  pinout · driver_input · driver_output      │
│  driver_wifi · hardware                     │
└─────────────────────────────────────────────┘
```

The main loop is fully non-blocking — no `delay()` calls anywhere:

```
Loop tick ──→ Network::update() → Actions::check()
              → Leds::update() → Buttons::update()
              → Blinds::update() (or Lights::update())
```

Everything is driven by timing comparisons against `millis()`. The motor state machine runs every cycle, accumulating position from elapsed time. Buttons are polled with debounce built into the press-duration measurement. The MQTT client reconnects automatically with backoff.

Each layer has its own detailed documentation in the [docs/](docs/) folder — architecture, pinouts, network protocol, and the build guide are all there.

## What's managed here

| What | How |
|---|---|
| **Motor control** | State machine with position tracking, configurable timing, and hardware relay interlocks |
| **Binary MQTT protocol** | Compact command/response protocol over PubSubClient with automatic reconnect and LWT |
| **Button handling** | Triple-button input classified as short / medium / long press, mapped to device actions |
| **LED feedback** | Dual-color LEDs showing network status, connection state, and device mode |
| **Position tracking** | Real-time accumulation using elapsed time and calibrated travel duration per direction |
| **OTA updates** | Fetches firmware binaries from the local network and applies them wirelessly |
| **WiFi provisioning** | Falls back to an Access Point with a web form for entering network credentials |
| **Device discovery** | Responds to MQTT broadcast with staggered timing, ready for backend provisioning |

## MQTT protocol & data flow

Devices communicate with the backend over MQTT using a compact binary protocol. State updates are 2 bytes, commands are single-byte opcodes, and the info response is 15 bytes packed with `struct`.

**Operational commands** (received on `tp/{ID}/c`):

| Byte | Command | Action |
|---|---|---|
| `0x00`–`0x64` | Set position | Go to position (0–100%) |
| `0xC0` | UP | Go to fully open |
| `0xC1` | DOWN | Go to down-position or fully closed |
| `0xC2` | STOP | Halt motor immediately |
| `0xC3` | PING | Publish heartbeat |
| `0xC4` | STATE | Publish current position and motor state |

A typical command flow from the frontend all the way down to the chip:

```
Frontend sends POST /commands/B0101/down
        │
        ▼
API publishes 0xC1 to tp/B0101/c via MQTT
        │
        ▼
commands.h parses 0xC1 → calls Blinds::down()
        │
        ▼
blinds.h activates relays respecting invertedRelays
motor starts, realPosition accumulates per tick
        │
        ▼
Every 1s: publishes 2-byte state to tp/B0101/s
On stop: saves position to NVS, publishes final state
```

The device publishes its state on `tp/{id}/s` as two bytes: `position` (0–100) and `motor_state` (0=idle, 1=waiting, 2=moving, 3=stopping). On unexpected disconnect, the MQTT broker publishes a Last Will with byte `0xFF` so the backend marks the device offline.

## Supported hardware

| Device | Chip | Type | Status |
|---|---|---|---|
| Matismo WIP100 | TYWE3S (ESP8266) | Blind controller | **Stable** |
| Matismo WIP100 | CB3S (BK7231N) | Blind controller | In progress |
| BSeed Melody M1 | T34 (BK7231N) | Light switch | In progress |

## Tech stack

| Layer | Technology |
|---|---|
| **Language** | C++17 |
| **Framework** | Arduino (ESP8266 Core / LibreTiny for BK7231N) |
| **Build system** | PlatformIO |
| **MQTT client** | PubSubClient |
| **Persistence** | Preferences (NVS) |
| **Flashing tools** | esptool / ltchiptool |

## How to build

See **[docs/build.md](docs/build.md)** for the full setup guide — wiring diagrams, bootloader entry, and per-environment commands.

```bash
# Clone the repo
git clone https://github.com/mtaibo/tphome-firmware
cd tphome-firmware

# Copy and fill in your WiFi and MQTT credentials
cp include/settings/credentials.example.h include/settings/credentials.h

# Build and flash — example: ESP8266 blind controller
pio run -e blind_esp8266 -t upload
```

Three build environments are available:

| Environment | Chip | Device type |
|---|---|---|
| `blind_esp8266` | ESP8266 | Blind controller |
| `blind_bk7231n` | BK7231N | Blind controller |
| `light_bk7231n` | BK7231N | Light switch |

## License

MIT — see [LICENSE](LICENSE)
