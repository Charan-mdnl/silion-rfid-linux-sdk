#!/usr/bin/env python3
"""
Continuous RFID Tag Monitor
Listens on /dev/ttyUSB0 at 38400 baud and displays tag data
Press Ctrl+C to stop
"""
import serial
import time
import sys

PORT = '/dev/ttyUSB0'
BAUD = 38400  # Detected as likely RFID protocol baud rate

print("=" * 70)
print("   🏷️  CONTINUOUS RFID TAG MONITOR 🏷️")
print("=" * 70)
print(f"\nPort: {PORT}")
print(f"Baud Rate: {BAUD}")
print("\n📡 Listening for RFID tags... (Press Ctrl+C to stop)")
print(f"{'=' * 70}\n")

try:
    ser = serial.Serial(PORT, BAUD, timeout=0.1)
    time.sleep(0.5)  # Allow serial port to stabilize
    
    tag_count = 0
    
    while True:
        if ser.in_waiting > 0:
            data = ser.read(ser.in_waiting)
            if data and len(data) > 0:
                tag_count += 1
                timestamp = time.strftime("%H:%M:%S")
                
                print(f"[{timestamp}] Tag #{tag_count} Detected:")
                print(f"  Length: {len(data)} bytes")
                
                # Show HEX
                hex_str = ' '.join(f'{b:02X}' for b in data)
                print(f"  HEX: {hex_str}")
                
                # Try ASCII
                try:
                    ascii_str = data.decode('ascii', errors='replace')
                    if ascii_str.isprintable() or '\n' in ascii_str or '\r' in ascii_str:
                        print(f"  ASCII: {repr(ascii_str)}")
                except:
                    pass
                
                # Look for EPC patterns (common RFID tag format)
                if len(data) >= 12:
                    print(f"  Possible EPC: {data.hex().upper()}")
                
                print("-" * 70)
        
        time.sleep(0.01)  # Small delay to prevent CPU spinning

except KeyboardInterrupt:
    print(f"\n\n✅ Stopped. Total tags detected: {tag_count}")
    print("=" * 70)
    
except Exception as e:
    print(f"\n❌ Error: {e}")
    sys.exit(1)
    
finally:
    try:
        ser.close()
    except:
        pass
