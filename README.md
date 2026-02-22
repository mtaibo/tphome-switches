# 🏠 TPHome Switches

**TPHome** is a personal project developed as a hands-on learning initiative during my first year of University. It was born from a practical necessity: to eliminate the fragmentation of smart home ecosystems and bypass the functional limitations of proprietary applications.

The project began by integrating Tuya devices into a **Home Assistant** environment. While this centralization was effective, I sought a deeper understanding of the hardware-software interface. I wanted to implement specific behaviors—such as granular blind synchronization and custom lighting logic—that stock firmwares often restrict.

**TPHome Switches** represents my journey into firmware development. It is an open-source collection of custom firmwares designed to replace factory software on commercial chips (found in brands like **BSEED** or **Matismo**). By transitioning to a local-first, MQTT-based architecture, this project serves as a sandbox for implementing:

* **Custom Event-Driven Logic:** Implementation of multi-state interrupt handling for physical buttons, supporting Short, Medium, and Long press events for advanced device interaction.
* **Time-Based Positioning Engine:** Developed a custom algorithm to track and calculate blind position in real-time. It manages independent calibration for ascent and descent travel times to ensure high precision in partial opening states.
* **Hardware Integrity & Safety:** Engineered software-level interlocks to prevent simultaneous relay activation and integrated motor rest intervals to protect hardware longevity.
* **Local-First Architecture:** Implemented direct MQTT communication to ensure low-latency control and complete independence from third-party cloud dependencies.

> **NOTE:** This is a **personal learning project** in continuous evolution. It is tailored for my specific home setup and hardware revisions. While it demonstrates core engineering principles, it is not intended as a universal "plug-and-play" solution; careful review of the hardware mapping is required before deployment.

---

### 📑 Supported Devices & Hardware

#### 🪟 Blind Controllers
* [**Matismo WIP100 (TYWE3S/ESP8266)**](#-matismo-wip100--tywe3s-esp8266) - Description. *[Current Stable; New Features Upcoming]*
* [**Matismo WIP100 (CB3S/BK7231N)**](#-matismo-wip100--cb3s-bk7231n) - Description. *(Upcoming support)*

#### 💡 Light Switches
* [**BSeed Melody M1 (T34/BK7231N)**](#-bseed-melody-m1--t34-bk7231n) - Description. *(Upcoming support)*

---

# 🔧 Matismo WIP100 | TYWE3S (ESP8266)

The **TYWE3S** is a low-power 32-bit CPU commonly found in Tuya devices. This documentation details the pinout configuration and logic for the chip when used for blind motor control.

**Quick Navigation:**

[📍 Pin Distribution](#-pin-distribution) | [⚙️ Hardware Logic](#-hardware-logic) | [🖱️ Input Actions](#-input-actions) | [📡 MQTT Interface](#-mqtt-interface)

--- 

### 📍 Pin Distribution

The following table maps the available pins for the **Matismo** blind configuration:

> **Note:** Pins 1 and 3 are reserved for Serial Communication (TX/RX). Avoid using them for other tasks if you need to see the Serial Monitor. Also pins 6 to 11 are dedicated to internal flash memory; using them as general GPIO pins may cause the chip to crash.


| Function | 0 | 1 | 2 | 3 | 5 | 12 | 13 | 14 | 15 | 16 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **LEDs** | Green | | Low | | | | | Mid | | Top |
| **Buttons**| | | | Mid | Low | Top | | | | |
| **Relays** | | | | | | | **L1** | | **L3** | |
| **TX/RX** | | **X** | | **X** | | | | | | |

---

### ⚙️ Hardware Logic

#### 💡 LEDs
* **Green LED (Pin 0):** Simple network state indicator.
    * `HIGH`: LED On (Green).
    * `LOW`: LED Off.
* **Status LEDs (Mid/Top/Low):** These are dual-color LEDs.
    * `HIGH`: **Red** (Busy/Error).
    * `LOW`: **Blue** (Idle/OK).

#### 🖱️ Buttons (Active Low)
All buttons follow standard pull-up logic:
* **Pressed:** `LOW` (0).
* **Released:** `HIGH` (1).

#### 🔌 Relays (Blind Motor Control)
Relays control the current flow to the motor.
* `HIGH`: Relay Active (Current ON).
* `LOW`: Relay Inactive (Current OFF).
* **Assignments:** Pin 13 is mapped to **L1** and Pin 15 to **L3**.

---

### 🖱️ Input Actions

Button behavior is determined by the duration of the press.

| Button | Short Press | Medium Press (3s) | Long Press (10s) |
| :--- | :--- | :--- | :--- |
| **Top** | `blind_up` | *(Unassigned)* | `access_point` |
| **Middle** | `blind_stop` | `save_config` | `reset_memory` |
| **Bottom** | `blind_down` | *(Unassigned)* | `network_setup` |

---




### 📡 MQTT Interface

The project follows a standardized **Topic** architecture to ensure consistency across all devices. Each device subscribes to a command path and broadcasts its status through a telemetry path. All this communications goes through two different channels.

#### 1. Communication Channels | Topic structure
This structure manages two paths to separate high-level actions from low-level system maintenance:

| Channel | Path Pattern | 
| :--- | :--- | 
| **Main (Operational)** | `tphome/[device_type]/[room]/[device_name]/...` | 
| **Admin (System maintenance)** | `tphome/admin/[device_id]/...` | 

* **Commands (`/set`):** The device listens for incoming instructions on this suffix.
* **Telemetry (`/state`):** The device publishes real-time feedback, confirmations, or diagnostic data on this suffix.

#### 2. Action & Payload Reference
The following payloads are valid when sent to a **`/set`** topic. The device will acknowledge actions by publishing the result to the corresponding **`/state`** topic.

**A. Operational Commands (Main Channel)**
| Command | Action | State Response (Example) |
| :--- | :--- | :--- |
| `UP` | Triggers the ascent relay. | `MOVING`, `OPEN`, `100` |
| `DOWN` | Triggers the descent relay. | `MOVING`, `CLOSED`, `0` |
| `STOP` | Halts motor and saves current position. | `STOPPED`, `[0-100]` |
| `SET:60` | Moves to a specific calculated percentage. | `MOVING` → `[Position]` |

**B. System Commands (Admin Channel)**
| Command | Action | State Response (Example) |
| :--- | :--- | :--- |
| `REBOOT` | Performs a soft reset of the MCU. | `SYSTEM_RESTARTING` |
| `SAVE_CONFIG` | Commits current parameters to NVS memory. | `CONFIG_SAVED` |
| `FACTORY_RESET` | Wipes all stored memory and reboots. | `MEMORY_CLEARED` |
| `GET_STATUS` | Requests a full telemetry diagnostic. | `V:[ver] IP:[ip] RSSI:[db]` |

---

### ⚙️ Logic & Safety Implementation
* **Time-Based Positioning:** The *Positioning Engine* calculates real-time displacement based on independent ascent/descent calibration.
* **Directional Delay:** A safety guard-time is enforced when switching between `UP` and `DOWN` to prevent motor stress and back-EMF.
* **NVS Persistence:** Current position and critical configurations are stored in **Non-Volatile Storage (NVS)**, ensuring the device resumes its correct state after a power cycle.
* **Asynchronous Execution:** MQTT polling and hardware control are handled via non-blocking logic to ensure system responsiveness during motor operation.

[↑ Back to Top](#-tphome-switches)


---

# 🔧 Matismo WIP100 | CB3S (BK7231N)
*(Development in progress...)*

---

# 🔧 BSeed Melody M1 | T34 (BK7231N)
*(Development in progress...)*
