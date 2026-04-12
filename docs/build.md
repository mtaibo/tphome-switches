# Build & Flash

---

## Requirements

- [PlatformIO](https://platformio.org/) — CLI or IDE extension (VS Code recommended)
- A USB-TTL adapter (3.3V logic) for the initial flash
- The chip must be in bootloader mode during the first flash (see per-device notes below)

---

## Credentials Setup

Network credentials are not committed to the repository. Before building, copy the example file and fill it in:

```bash
cp include/settings/credentials.example.h include/settings/credentials.h
```

Edit `credentials.h` with your network details:

```cpp
#define WIFI_SSID   "your_wifi_ssid"
#define WIFI_PASS   "your_wifi_password"
#define MQTT_IP     "192.168.x.x"
#define MQTT_PORT   1883
#define MQTT_USER   "your_mqtt_user"
#define MQTT_PASS   "your_mqtt_password"
```

These values are used as the compile-time defaults loaded on first boot. After that, credentials are stored in NVS flash and can be updated via the Access Point provisioning flow without reflashing.

> `credentials.h` is listed in `.gitignore` — it will not be committed accidentally.

---

## Build Environments

Environments are defined in `platformio.ini`. Each environment targets a specific chip and device type combination:

| Environment | Chip | Device type |
|---|---|---|
| `blind_esp8266` | ESP8266 (TYWE3S) | Blind controller |
| `blind_bk7231n` | BK7231N (CB3S) | Blind controller |
| `light_bk7231` | BK7231N (T34) | Light switch |

The default environment is `blind_esp8266`.

---

## Building

```bash
# Build default environment (blind_esp8266)
pio run

# Build a specific environment
pio run -e blind_bk7231n

# Build all environments
pio run -e blind_esp8266 -e blind_bk7231n -e light_bk7231
```

The compiled binary is placed at `.pio/build/{env}/firmware.bin`.

---

## Flashing

### Initial flash via USB-TTL

Connect the USB-TTL adapter to the chip's TX, RX, GND and 3.3V pins. Put the chip in bootloader mode (GPIO 0 pulled to GND on power-up for ESP8266).

```bash
# Build and upload in one step
pio run -e blind_esp8266 -t upload

# Specify port manually if auto-detection fails
pio run -e blind_esp8266 -t upload --upload-port /dev/ttyUSB0
```

### Serial monitor

```bash
pio device monitor --baud 115200
```

Note that on the Matismo WIP100 (ESP8266), GPIO 1 (TX) is shared with `LED_CONFIG`. The serial output will interfere with the blue LED backlight while the monitor is open.

### Subsequent flashes via OTA

Once a device is running and connected to the network, firmware updates can be pushed wirelessly via the MQTT admin command `0xA0` (`OTA`). The device will fetch the binary from:

```
http://{mqttIP}/firmware/{deviceID}.bin
```

Host the compiled `.bin` file on an HTTP server running on the same machine as the MQTT broker (the `tphome-api` backend handles this). The device reboots automatically after a successful update.

---

## First Boot Behaviour

On first boot (or after a factory reset), the device initialises its NVS with the compiled-in defaults from `credentials.h` and `defaults.h`:

- Device ID is derived from the last 4 hex digits of the MAC address
- WiFi and MQTT credentials come from `credentials.h`
- Blind travel times default to 10s up / 10s down
- Default down position is 20%
- Starting position is 100% (fully open)

The device then enters `CONNECTION` mode and attempts to connect to WiFi and MQTT. Once connected, it publishes its ID to the backend for registration and transitions to `NORMAL` mode.

If connection fails after 5 attempts, the device stops retrying. Use `BTN_TOP` short press (in `CONNECTION` mode) to trigger a manual reconnect, or `BTN_MID` short press to start the Access Point provisioning portal.

---

## Button Reference (CONNECTION mode)

When in `CONNECTION` mode the buttons have network-management functions instead of device-control functions:

| Button | Short press | Action |
|---|---|---|
| Top | Reconnect | Retry WiFi + MQTT if disconnected, or return to NORMAL if already connected |
| Mid | AP portal | Start Access Point provisioning web server |
| Btm | Reboot | Soft reset the MCU |
