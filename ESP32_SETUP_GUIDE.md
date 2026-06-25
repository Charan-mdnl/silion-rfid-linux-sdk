# ESP32 RFID Standalone Setup Guide

## 🎯 Goal
Make ESP32 read RFID tags **independently** (no laptop), then send to phone via WiFi.

---

## 📋 Upload Instructions

### Step 1: Find the RIGHT Port

You have **TWO** USB devices:
- `/dev/ttyUSB0` = ❌ **RFID Module** (DON'T upload here!)
- `/dev/ttyUSB?` or `/dev/ttyACM?` = ✅ **ESP32** (Upload HERE!)

**In Arduino IDE:**
1. Disconnect ESP32 from USB
2. Tools → Port (note which ports you see)
3. Connect ESP32 to USB
4. Tools → Port (new port = your ESP32!)

---

### Step 2: Upload the Code

1. **Open:** `/home/charan/Downloads/NEWRFID-LINUX-C/esp32_rfid_standalone/esp32_rfid_standalone.ino`

2. **Select Board:**
   - Tools → Board → ESP32 Arduino → **ESP32 Dev Module**

3. **Select Port:**
   - Tools → Port → **(Your ESP32 port, NOT /dev/ttyUSB0)**

4. **Upload:**
   - Click Upload (→)
   - Wait for "Done uploading"

---

### Step 3: Test WITHOUT Laptop

1. **Disconnect** ESP32 from laptop
2. **Connect power** to ESP32 (via USB charger or power bank)
3. **Watch the built-in LED:**
   - 3 fast blinks = System ready ✅
   - 2 blinks when tag detected = Reading tags! 🏷️
   - Small pulse every 5 seconds = System alive 💚

4. **Scan RFID tag** near antenna
   - LED should blink twice!

---

## 🔧 Wiring Checklist

```
RFID Module → ESP32
TX  → GPIO 16
RX  → GPIO 17
GND → GND
VCC → 3.3V (NOT 5V!)
```

**IMPORTANT:** RFID connects to GPIO 16/17, **NOT TX0/RX0**!

---

## 🐛 Troubleshooting

### No output in Serial Monitor?

**Problem:** Wrong port selected OR code not uploaded

**Solution:**
1. Verify ESP32 port (see Step 1)
2. Make sure upload says "Done uploading"
3. Open Serial Monitor at **115200 baud**

---

### LED not blinking?

**Check:**
1. ✅ Code uploaded successfully?
2. ✅ ESP32 has power (USB or battery)?
3. ✅ LED pin 2 is correct for your board?

Some boards use different LED pins. If no blink, try changing line:
```cpp
const int LED_PIN = 2;  // Try: 2, 5, or 22
```

---

### RFID not working?

**Check:**
1. ✅ Wires connected to **GPIO 16/17** (not TX0/RX0)?
2. ✅ RFID module powered (3.3V, not 5V)?
3. ✅ Tag close to antenna?
4. ✅ Baud rate is 38400?

---

## 📱 Next Step: WiFi to Phone

Once the LED blinks when scanning tags, you're ready for WiFi!

The code is already prepared with helper functions:
- `getLastTagData()` - Returns tag hex data
- `getTagCount()` - Returns number of tags

**Your existing Blynk code can use these!**

Check: `/home/charan/Downloads/NEWRFID-LINUX-C/blynk_38400/blynk_38400.ino`

---

## ✅ Success Checklist

- [ ] ESP32 port identified (not /dev/ttyUSB0)
- [ ] Code uploaded successfully
- [ ] LED blinks 3 times on power-up
- [ ] LED blinks 2 times when tag scanned
- [ ] Works without laptop connected
- [ ] Ready to add WiFi!
