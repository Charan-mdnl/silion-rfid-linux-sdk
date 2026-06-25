# ESP32 & Linux C RFID Integration Suite (Silion Tech UHF RFID)

This repository contains a comprehensive suite of firmware, bridge scripts, and native C applications for integrating **Silion Tech UHF RFID Readers** with **ESP32 Microcontrollers**, **Linux Computers**, and the **Blynk IoT Platform**.

---

## 🚦 Project Status Summary

This project was built using **three main approaches**. To help you get started quickly without repeating past mistakes, here is the status of each approach:

| Approach | Connection Type | Target Platform | Status | Description |
| :--- | :--- | :--- | :--- | :--- |
| **Approach 1: Ethernet Cable** | RJ45/Network Port | Linux (C API) | **✅ WORKING** | Maximum performance, stable inventory, uses precompiled `.so` library. |
| **Approach 2: USB-C Serial** | USB-C / Serial | Linux (C & Python) | **✅ WORKING** | Verified serial communication over USB-C (`/dev/ttyACM0` or `/dev/ttyUSB0`). |
| **Approach 3: ESP32 Standalone** | UART (GPIO 16/17) | ESP32 (Arduino IDE) | **❌ NOT WORKING** | Experimental. Struggles with stable serial baud configs and Blynk cloud sync. |

---

## 📂 Repository Directory Structure

Below is the directory mapping, showing which sections of code are verified working and which are experimental/non-working:

```text
├── demos/                       # ✅ WORKING ── Linux C demo programs from Silion Tech
│   ├── 普通模式盘点/             # ✅ WORKING ── Normal mode inventory (Default: Ethernet IP 192.168.1.100)
│   ├── 快速模式盘点/             # ✅ WORKING ── Fast mode inventory (Impinj Fast Mode)
│   └── 事件模式盘点/             # ✅ WORKING ── Callback/Event mode inventory
├── libs-glibc2.19/              # ✅ WORKING ── Precompiled Linux C API libraries (.so)
│   ├── x86/                     # 32-bit Linux x86 systems
│   ├── x64/                     # 64-bit Linux x86_64 systems
│   └── arrch64/                 # 64-bit ARM systems (e.g. Raspberry Pi, Jetson Nano)
├── read_tags.c                  # ✅ WORKING ── Custom simplified Linux C serial reader CLI
├── try_connect.c                # ✅ WORKING ── Custom simplified Linux C serial port connection tester
├── run_reader.sh                # ✅ WORKING ── Helper script to run read_tags with correct x64 library path
├── run_rfid_demo.sh             # ✅ WORKING ── Helper script to compile and run Silion Tech's normal mode demo
├── read_rfid_simple.py          # ✅ WORKING ── Python script to auto-scan baud rates and read RFID raw hex
├── monitor_rfid.py              # ✅ WORKING ── Python script for general RFID serial monitoring
├── docs/                        # ✅ WORKING ── API & User Guides in English & Chinese
│
├── rfid_to_esp32.py             # ⚠️ PARTIALLY WORKING ── Python bridge script (requires working ESP32 endpoint)
│
├── esp32_standalone_final/      # ❌ NOT WORKING ── Standalone sketch (RFID → ESP32 → Blynk over WiFi)
├── esp32_http_blynk/            # ❌ NOT WORKING ── HTTP Server sketch (Python Bridge → ESP32 → Blynk)
├── blynk_universal/             # ❌ NOT WORKING ── Blynk test sketch (38400 Baud, lock-in config)
├── blynk_38400/                 # ❌ NOT WORKING ── Blynk integration at 38400 baud
├── blynk_rfid/                  # ❌ NOT WORKING ── Blynk integration test
├── blynk_rfid_final/            # ❌ NOT WORKING ── Blynk integration final
├── blynk_rfid_raw/              # ❌ NOT WORKING ── Blynk raw serial read
├── blynk_scanner/               # ❌ NOT WORKING ── Blynk scanner test
├── blynk_smart/                 # ❌ NOT WORKING ── Blynk smart integration
├── scanner_serial/              # ❌ NOT WORKING ── ESP32 Serial scanner utility to find correct baud/config
├── auto_detect_rfid/            # ❌ NOT WORKING ── ESP32 auto-detection sketch
├── rfid_debug/                  # ❌ NOT WORKING ── ESP32 serial debug sketch
├── rfid_display/                # ❌ NOT WORKING ── ESP32 display integration sketch
└── debug_serial/                # ❌ NOT WORKING ── ESP32 general serial debugging sketch
```

---

## 🔌 Hardware wiring Guide

### For Approach 1 (Ethernet) & Approach 2 (USB-C) [WORKING]
* Connect the RFID module to your laptop/PC using either an Ethernet cable (to the router/switch) or a USB-to-Serial converter cable (e.g. CP2102 or CH340).

### For Approach 3 (ESP32 Standalone) [NOT WORKING / EXPERIMENTAL]
If you want to debug or develop the ESP32 approach further, wire the modules as follows:

| RFID Module Pin | ESP32 GPIO Pin | Description |
|:---|:---|:---|
| **TX** | **GPIO 16 (RX2)** | RFID Serial Transmit → ESP32 Serial Receive |
| **RX** | **GPIO 17 (TX2)** | RFID Serial Receive → ESP32 Serial Transmit |
| **GND** | **GND** | Common Ground |
| **VCC** | **3.3V** | Power Source |

> [!WARNING]
> Keep the RFID module's USB-to-Serial cable **unplugged** from your computer while uploading code to the ESP32 to prevent serial bus sharing issues and conflicts on the serial line.

---

## 🚀 Setup Guide for Working Approaches

### 💻 Approach 1: Ethernet Connection (Linux C Demos)
The official demos are configured to connect to the RFID reader using an IP address (default: `192.168.1.100`) over Ethernet.

1. **Install Build Tools**:
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential gcc make
   ```
2. **Setup Library Path**:
   Go to the demo directory, clean and compile:
   ```bash
   cd demos/普通模式盘点
   make clean
   make
   ```
3. **Run the Program**:
   Export the library path and run the compiled binary:
   ```bash
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
   ./ReadTags 192.168.1.100  # Pass the reader's IP address
   ```

---

### 🔌 Approach 2: USB-C Serial Connection (Linux C & Python)

#### Option A: Running the C Utilities
1. Compile the custom C files in the root folder:
   ```bash
   # Compile connection test
   gcc try_connect.c -o try_connect -L./libs-glibc2.19/x64 -lModuleAPI -I.
   
   # Compile tag reader CLI
   gcc read_tags.c -o read_tags -L./libs-glibc2.19/x64 -lModuleAPI -I.
   ```
   *(Note: If building on Raspberry Pi, change the library path from `x64` to `arrch64`)*
2. Assign serial port permissions:
   ```bash
   sudo chmod 666 /dev/ttyACM0
   ```
3. Run the helper script to execute the tag reader:
   ```bash
   chmod +x run_reader.sh
   ./run_reader.sh
   ```

#### Option B: Running the Python Utilities
1. Install Python `pyserial`:
   ```bash
   pip3 install pyserial
   ```
2. Run the simple baud-rate scanning reader:
   ```bash
   python3 read_rfid_simple.py
   ```
   *This script tries multiple standard baud rates (9600, 38400, 57600, 115200) to detect the RFID reader on `/dev/ttyUSB0` or `/dev/ttyACM0`.*

---

## ⚠️ Notes on the Failed Approach 3 (ESP32 & Blynk)

During development, the ESP32 standalone and HTTP server approaches failed to function reliably due to:
1. **Serial Frame Inconsistencies:** The RFID reader uses a high-frequency custom frame-based serial protocol. ESP32's `HardwareSerial` struggled to parse and frame-align the hex packets reliably.
2. **Logic Level/Inversion Issues:** The RFID module's serial output was found to use inverted or non-standard stop-bit configurations in some modes, leading to packet loss or `[00]` idle byte corruption.
3. **Blynk Latency:** Blocking operations during Blynk cloud communication (`Blynk.run()`) caused the ESP32 to drop serial frames from the RFID module during high-rate scans.

If you are a developer looking to fix this approach:
* Look into the [scanner_serial/scanner_serial.ino](file:///home/charan/Downloads/NEWRFID-LINUX-C/scanner_serial/scanner_serial.ino) file, which attempts to auto-detect inverted configurations.
* Use a hardware logic analyzer or oscilloscope on GPIO 16/17 to inspect the frame waveforms.
* Avoid blocking loop code to ensure the serial buffer on UART2 does not overflow.
