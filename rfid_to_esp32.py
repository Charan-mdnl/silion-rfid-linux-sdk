# ⚠️ STATUS: PARTIALLY WORKING / UNSTABLE
# This Python script reads RFID tags from serial and sends them to the ESP32 via HTTP.
# Because the ESP32/Blynk approach did not work reliably, this bridge is unstable.
# Use read_rfid_simple.py or C-based approaches for verified working solutions.

#!/usr/bin/env python3
"""
RFID → ESP32 Bridge
Reads RFID tags from /dev/ttyUSB0 and sends to ESP32 via HTTP
"""

import serial
import time
import requests
import sys
from datetime import datetime

# Configuration
RFID_PORT = '/dev/ttyUSB0'
RFID_BAUD = 38400
ESP32_IP = "192.168.1.100"  # Change this to your ESP32's IP

# Check for ESP32 IP argument
if len(sys.argv) > 1:
    ESP32_IP = sys.argv[1]

ESP32_URL = f"http://{ESP32_IP}/tag"

print("=" * 70)
print("   🏷️  RFID → ESP32 → BLYNK BRIDGE 🏷️")
print("=" * 70)
print()
print(f"RFID Port: {RFID_PORT}")
print(f"Baud Rate: {RFID_BAUD}")
print(f"ESP32 IP:  {ESP32_IP}")
print(f"ESP32 URL: {ESP32_URL}")
print()
print("=" * 70)
print()

# Test ESP32 connectivity
print("🔍 Testing ESP32 connection...")
try:
    response = requests.get(f"http://{ESP32_IP}/", timeout=5)
    if response.status_code == 200:
        print("✅ ESP32 is reachable!")
    else:
        print(f"⚠️  ESP32 responded with status {response.status_code}")
except Exception as e:
    print(f"❌ Cannot reach ESP32 at {ESP32_IP}!")
    print(f"   Error: {e}")
    print()
    print("💡 Make sure:")
    print("   1. ESP32 is powered on and running the sketch")
    print("   2. ESP32 is connected to WiFi")
    print("   3. You're using the correct IP address")
    print()
    sys.exit(1)

print()

# Open RFID serial port
print(f"📡 Opening RFID port {RFID_PORT}...")
try:
    ser = serial.Serial(RFID_PORT, RFID_BAUD, timeout=0.1)
    print("✅ RFID port opened successfully!")
except Exception as e:
    print(f"❌ Failed to open RFID port!")
    print(f"   Error: {e}")
    print()
    print("💡 Make sure:")
    print("   1. RFID module is connected via USB")
    print("   2. /dev/ttyUSB0 exists (run: ls /dev/ttyUSB*)")
    print("   3. You have permissions (run: sudo chmod 666 /dev/ttyUSB0)")
    print()
    sys.exit(1)

print()
print("=" * 70)
print("📡 Listening for RFID tags... (Press Ctrl+C to stop)")
print("=" * 70)
print()

tag_count = 0
last_tag_time = 0

try:
    while True:
        if ser.in_waiting > 0:
            # Read available data
            data = ser.read(ser.in_waiting)
            
            # Avoid duplicate reads (debounce)
            current_time = time.time()
            if current_time - last_tag_time < 0.5:
                continue
            
            # Convert to hex string
            hex_data = ' '.join([f'{b:02X}' for b in data])
            
            # Filter: only process if we got meaningful data (at least 4 bytes)
            if len(data) >= 4:
                tag_count += 1
                timestamp = datetime.now().strftime("%H:%M:%S")
                
                print(f"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
                print(f"🏷️  TAG #{tag_count} | {timestamp}")
                print(f"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
                print(f"Raw HEX: {hex_data}")
                print(f"Bytes:   {len(data)}")
                
                # Send to ESP32
                try:
                    response = requests.get(
                        ESP32_URL,
                        params={'epc': hex_data},
                        timeout=2
                    )
                    
                    if response.status_code == 200:
                        print(f"✅ Sent to ESP32 → Blynk")
                    else:
                        print(f"⚠️  ESP32 returned status {response.status_code}")
                        print(f"   Response: {response.text}")
                        
                except requests.exceptions.Timeout:
                    print(f"⏱️  ESP32 timeout (tag may still be received)")
                except Exception as e:
                    print(f"❌ Failed to send to ESP32: {e}")
                
                print()
                
                last_tag_time = current_time
        
        time.sleep(0.01)  # Small delay to avoid CPU spinning

except KeyboardInterrupt:
    print()
    print("=" * 70)
    print("🛑 Stopped by user")
    print(f"📊 Total tags detected: {tag_count}")
    print("=" * 70)
    ser.close()
    sys.exit(0)
except Exception as e:
    print(f"\n❌ Error: {e}")
    ser.close()
    sys.exit(1)
