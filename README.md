# TPHome Firmware

**TPHome** is a personal home automation project built from the ground up — hardware flashing, custom firmware, local MQTT backend, and a web frontend — with the goal of replacing proprietary smart home apps with a fully self-hosted, local-first stack.

This repository contains the **firmware** layer: custom C++ code that runs directly on the chips inside commercial smart switches and blind controllers, replacing their factory software.

> **Note:** This is a personal project tailored to my specific hardware. It is not a plug-and-play solution — hardware pin mappings must be reviewed before deploying on different boards.

---

## TPHome Ecosystem

| Repository | Description | Status |
|---|---|---|
| **tphome-firmware** | Firmware for ESP8266 / BK7231N chips | Active |
| [`tphome-api`](https://github.com/mtaibo/tphome-api) | FastAPI + MQTT backend on Raspberry Pi | In progress |
| `tphome` | Frontend + full system orchestration | Planned |

---

## Custom firmware

Commercial smart home devices (Tuya, BSEED, Matismo...) come locked to vendor clouds with limited configurability. This firmware replaces the factory software to provide:

- **Local-first control** via MQTT — no cloud dependency, no latency
- **Granular blind positioning** with independent up/down calibration
- **Custom button logic** with short, medium and long press events
- **OTA updates** served from the local network
- **Hardware safety interlocks** to protect motors

---

## Supported Devices

| Device | Chip | Type | Status |
|---|---|---|---|
| Matismo WIP100 | TYWE3S (ESP8266) | Blind controller | Stable |
| Matismo WIP100 | CB3S (BK7231N) | Blind controller | In progress |
| BSeed Melody M1 | T34 (BK7231N) | Light switch | In progress |

---

## Project Structure

```
tphome-firmware/
├── src/
│   ├── main.cpp          # Entry point — setup() and loop()
│   └── settings.cpp      # NVS persistence logic
├── include/
│   ├── hardware/         # Low-level drivers and pin definitions
│   │   ├── esp8266/
│   │   └── bk7231n/
│   ├── logic/            # Device behavior (blinds, buttons, LEDs)
│   ├── network/          # WiFi, MQTT, OTA, AP provisioning
│   └── settings/         # Config structs, defaults, mode management
├── platformio.ini        # Build environments
└── docs/
    ├── architecture.md
    ├── hardware.md
    ├── network.md
    └── build.md
```

---

## Quick Start

See **[docs/build.md](docs/build.md)** for the full setup guide.

**Requirements:** PlatformIO CLI or IDE, USB-TTL adapter for initial flash

```bash
# Clone the repo
git clone https://github.com/mtaibo/tphome-firmware
cd tphome-firmware

# Copy and fill in your credentials
cp include/settings/credentials.example.h include/settings/credentials.h

# Build and flash — example: ESP8266 blind controller
pio run -e blind_esp8266 -t upload
```

---

## Documentation

| Document | Description |
|---|---|
| [Architecture](docs/architecture.md) | Module breakdown, execution flow, design decisions |
| [Hardware](docs/hardware.md) | Pinouts, relay logic, LED states per device |
| [Network & Protocol](docs/network.md) | MQTT topics, binary command protocol, provisioning flow |
| [Build & Flash](docs/build.md) | PlatformIO environments, credentials setup, flashing guide |

---

## License

MIT — see [LICENSE](LICENSE)
