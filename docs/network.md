# Network & Protocol

This document covers the MQTT topic structure, the binary command protocol, the device provisioning flow, and the other network features (OTA, Access Point).

---

## MQTT Topic Structure

Each device subscribes to and publishes on a set of topics constructed from its device ID at boot time. There are two operational channels:

| Channel | Purpose |
|---|---|
| **Operational** | Normal device control — movement commands, state reporting |
| **Admin** | System-level management — preferences, OTA, reboot, factory reset |

### Configured Devices

A device is considered configured once it has been assigned a full device ID (5 characters). Topics are built as:

| Topic | Direction | Description |
|---|---|---|
| `tp/{ID}/c` | Subscribe | Operational commands |
| `tp/{ID}/a` | Subscribe | Admin commands |
| `tp/{ID}/s` | Publish | Device state |
| `tp/a/c` | Subscribe | Global broadcast (all devices) |

**Device ID format:** `{type}{zone:02d}{device:02d}` — for example, `B0102` means blind (`B`), zone `01`, device `02`.

### Unconfigured Devices

On first boot, the device derives a temporary 4-character ID from the last two bytes of its MAC address (e.g. `A1B2`). It subscribes to a single topic until the backend assigns it a permanent ID:

| Topic | Direction | Description |
|---|---|---|
| `def/{MAC4}/a` | Subscribe | Awaiting ID assignment |
| `def/{MAC4}/s` | Publish | Identification payload |

On receiving a valid `DeviceID` struct on the `def` topic, the device constructs its permanent ID, saves it to NVS and reboots to build the correct operational topics.

---

## Command Protocol

All payloads are binary. The first byte always identifies the command. There are no text-based commands.

### Operational Commands (`tp/{ID}/c`)

Sent to the `/c` topic. Single-byte payloads.

| Byte value | Command | Action |
|---|---|---|
| `0x00`–`0x64` (0–100) | Set position | Move blind to percentage × 100 (0–10000 internal units) |
| `0xC0` | `UP` | Move to 100% (fully open) |
| `0xC1` | `DOWN` | Move to `downPosition` preference, or 0% if already at or below it |
| `0xC2` | `STOP` | Halt motor and save current position |
| `0xC3` | `PING` | Publish a single `0x01` byte to state topic |
| `0xC4` | `STATE` | Publish current state immediately |

> The `DOWN` command implements a two-step behaviour: first press goes to the configured `downPosition` (e.g. 20%), second press goes to 0% (fully closed). This mirrors the physical button behaviour.

### Admin Commands (`tp/{ID}/a`)

Sent to the `/a` topic. First byte is the command type, remaining bytes are the payload.

| Byte | Command | Payload | Description |
|---|---|---|---|
| `0xA0` | `OTA` | none | Trigger OTA update from `http://{mqttIP}/firmware/{deviceID}.bin` |
| `0xA1` | `REBOOT` | none | Soft reset the MCU |
| `0xA2` | `RESET_MEM` | none | Wipe NVS flash and reboot (factory reset) |
| `0xA3` | `SET_POS` | `uint16_t` (2 bytes, little-endian) | Override stored position (0–10000) |
| `0xA4` | `SET_PREFS` | `sizeof(Prefs)` bytes | Write full `Prefs` struct to NVS |

The `Prefs` struct layout for blind devices (packed, little-endian):

```
Offset  Size  Field
0       2     upTime         — ascent travel time in centiseconds
2       2     downTime       — descent travel time in centiseconds
4       2     downPosition   — default down stop position (0–10000)
6       1     invertedRelays — 0x00 or 0x01
```

Total: 7 bytes.

---

## State Publishing

The device publishes state to `tp/{ID}/s` (or `def/{MAC4}/s` if unconfigured).

### Blind state payload (2 bytes, packed)

```
Offset  Size  Field
0       1     position  — current position mapped to 0–100 (currentPosition / 100)
1       1     state     — motor state enum: 0=IDLE, 1=WAITING, 2=MOVING, 3=STOPPING
```

State is published:
- Every second while the motor is moving
- Immediately on stop
- On demand via `CMD::STATE` or `CMD::PING`

### LWT (Last Will and Testament)

On connect, the firmware registers a LWT on the state topic with payload `0xFF`. The broker publishes this automatically if the device drops its connection unexpectedly, allowing the backend to detect offline devices.

### Global broadcast

All devices subscribe to `tp/a/c`. When a message is received on this topic, every device publishes a `DeviceID` struct to its state topic after a random delay (50–1500ms) to avoid simultaneous collisions. This allows the backend to discover all online devices in one sweep.

---

## Device Provisioning Flow

```
First boot
    │
    ▼
Device derives 4-char ID from MAC
Subscribes to def/{MAC4}/a
Publishes DeviceID struct on global broadcast response
    │
    ▼
Backend sends DeviceID struct to def/{MAC4}/a
    │
    ▼
Device saves new 5-char ID to NVS
Device reboots
    │
    ▼
Subscribes to tp/{ID}/c, tp/{ID}/a, tp/a/c
Normal operation begins
```

If the device cannot connect to WiFi or MQTT on first boot, it can be provisioned through the Access Point mode (see below).

---

## Access Point Provisioning

If network credentials are not configured or are incorrect, the device can be put into AP mode to receive new credentials via a web form.

**Trigger:** `BTN_MID` short press while in `CONNECTION` mode.

The device creates a WiFi access point with its device ID as the SSID (e.g. `A1B2`). Connecting to it and navigating to `http://192.168.4.1` shows a form to enter:

- WiFi SSID and password
- MQTT broker IP, port, username and password

On submit, the device saves the credentials to NVS and reboots. AP mode runs a blocking loop — the device does not process any other events while the portal is active.

---

## OTA Updates

OTA is triggered by the `AdminCmd::OTA` command. The device fetches a binary from:

```
http://{mqttIP}/firmware/{deviceID}.bin
```

The update URL is built from the stored MQTT IP, which means the backend HTTP server must be running on the same host as the MQTT broker. All LEDs are turned off during the update. On success the device reboots into the new firmware automatically.

---

## WiFi & MQTT Reconnection Policy

Both WiFi and MQTT use the same conservative reconnection strategy:

- Maximum **5 attempts** per reconnection cycle
- **5 seconds** between each attempt
- After exhausting attempts, the module stops trying and waits for a manual reconnect

Manual reconnect is triggered by `BTN_TOP` short press in `CONNECTION` mode, or by `AdminCmd::REBOOT`.

This policy avoids hammering the network if credentials are wrong or the broker is down, while still recovering quickly from transient failures.
