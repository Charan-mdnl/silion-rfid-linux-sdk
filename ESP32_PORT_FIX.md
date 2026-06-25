# Quick Fix Guide - ESP32 Not Detected

## Problem
Only `/dev/ttyUSB0` is visible (your RFID module), but ESP32 is not showing up as a separate port.

## Possible Causes

### 1. ESP32 USB Cable Issue
- **Check:** Is your USB cable a DATA cable (not just power)?
- **Test:** Try a different USB cable
- **Test:** Try a different USB port on laptop

### 2. ESP32 Not in Programming Mode  
- **Try:** Press and hold BOOT button while plugging in ESP32
- **OR:** Press BOOT + RESET, release RESET, then release BOOT

### 3. CH340/CP2102 Driver Missing
ESP32 uses a USB-to-Serial chip. Check which one:
```bash
lsusb
```

Look for:
- `CH340` → Need CH340 driver
- `CP210x` → Need CP2102 driver  
- `FT232` → Need FTDI driver

### 4. ESP32 Using Same Chip as RFID Module
Both devices might share `/dev/ttyUSB0` at different times.

**Test this:**
1. Unplug RFID module USB
2. Keep RFID UART wires connected to ESP32
3. Check: `ls /dev/ttyUSB*`
4. If still shows `/dev/ttyUSB0`, that's your ESP32!

## Quick Upload Test

If `/dev/ttyUSB0` IS your ESP32:

1. **Close Serial Monitor** in Arduino IDE
2. **Unplug RFID module USB** (keep UART wires connected)
3. Upload code to ESP32 on `/dev/ttyUSB0`
4. Open Serial Monitor at 115200 baud
5. Watch for debug output!

---

## Try This Command:
```bash
# Unplug RFID USB, then run:
ls -la /dev/ttyUSB*
```

If `/dev/ttyUSB0` is still there → That'syour ESP32!  
If `/dev/ttyUSB0` disappears → ESP32 is not detected

---

## Your Setup Should Be:

```
┌─────────────┐  UART Wires   ┌─────────┐  USB Cable   ┌────────┐
│ RFID Module │──────────────→│  ESP32  │─────────────→│ Laptop │
└─────────────┘               └─────────┘              └────────┘
  TX → GPIO16                    (for upload
  RX → GPIO17                    & Serial Monitor)
  GND → GND
  VCC → 3.3V
  
(RFID USB should be UNPLUGGED during ESP32 programming!)
```
