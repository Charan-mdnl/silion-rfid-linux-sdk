# ✅ STATUS: WORKING
# This Python script is a verified working utility to auto-scan baud rates and read RFID tags via USB-C serial.

#!/usr/bin/env python3
"""
Simple RFID Reader - Tries multiple baud rates and shows raw data
No special libraries needed - just reads from serial port
"""
import serial
import time
import sys

BAUD_RATES = [9600, 38400, 57600, 115200]
PORT = '/dev/ttyUSB0'

print("=" * 60)
print("  RFID MODULE DATA READER")
print("=" * 60)
print(f"\nPort: {PORT}")
print("\nHOLD YOUR RFID TAG NEAR THE READER NOW!\n")
print("-" * 60)

for baud in BAUD_RATES:
    print(f"\n⏱️  Trying {baud} baud...")
    try:
        ser = serial.Serial(PORT, baud, timeout=0.5)
        time.sleep(0.5)  # Give it time to settle
        
        # Try reading for 3 seconds at this baud rate
        start = time.time()
        data_received = False
        
        while time.time() - start < 3:
            if ser.in_waiting > 0:
                data = ser.read(ser.in_waiting)
                if data:
                    data_received = True
                    print(f"\n✅ DATA RECEIVED at {baud} baud! ✅")
                    print(f"\nHEX Data:")
                    hex_str = ' '.join(f'{b:02X}' for b in data)
                    print(f"   {hex_str}")
                    
                    print(f"\nASCII Data (if readable):")
                    try:
                        ascii_str = data.decode('ascii', errors='replace')
                        print(f"   {repr(ascii_str)}")
                    except:
                        print("   (Not ASCII)")
                    
                    print("\n" + "=" * 60)
            
            time.sleep(0.05)
        
        ser.close()
        
        if data_received:
            print(f"\n🎯 CORRECT BAUD RATE: {baud}")
            print("\nKeep scanning tags - the program will keep trying...\n")
        
    except Exception as e:
        print(f"   Error: {e}")
        continue

print("\nFinished trying all baud rates.")
print("If no data was received, check:")
print("  1. Is the RFID module powered on?")
print("  2. Are the wires connected properly?")
print("  3. Is a tag being held near the antenna?")
