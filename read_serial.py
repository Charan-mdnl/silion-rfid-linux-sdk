import serial
import time
import sys

def read_serial(port='/dev/ttyACM0', baud=115200, duration=60):
    try:
        ser = serial.Serial(port, baud, timeout=1)
        print(f"Opened {port} at {baud}. Waiting for data...")
        print("Please press the RESET (EN) button on the ESP32 now...")
        start_time = time.time()
        
        while time.time() - start_time < duration:
            if ser.in_waiting:
                try:
                    data = ser.read(ser.in_waiting)
                    if data:
                        try:
                            decoded = data.decode('utf-8', errors='replace')
                            print(f"{decoded}", end='')
                        except:
                            print(f"{data.hex()}", end='')
                        sys.stdout.flush()
                except Exception as e:
                    print(f"Error reading: {e}")
            time.sleep(0.01)
            
    except Exception as e:
        print(f"Failed to open port: {e}")

if __name__ == "__main__":
    read_serial()
