import serial
import time
import sys

def scan_baud_rates(port='/dev/ttyACM0'):
    print(f"Scanning baud rates on {port}...")
    baud_rates = [9600, 19200, 38400, 57600, 115200, 230400, 460800]
    
    for baud in baud_rates:
        print(f"\n[+] Testing baud rate: {baud}")
        try:
            ser = serial.Serial(port, baud, timeout=0.5)
            # Clear buffer
            ser.reset_input_buffer()
            
            print(f"    Listening for 5 seconds. Please scan a tag now...")
            start_time = time.time()
            data_received = False
            
            while time.time() - start_time < 5:
                if ser.in_waiting:
                    data = ser.read(ser.in_waiting)
                    if data:
                        print(f"    !!! DATA RECEIVED !!!")
                        print(f"    Hex: {data.hex()}")
                        try:
                            print(f"    ASCII: {data.decode('utf-8', errors='replace')}")
                        except:
                            pass
                        data_received = True
                        break # Found data, but let's see if we can read more or move on? 
                              # Actually, if we find data, we should probably stop or at least highlight it.
                time.sleep(0.1)
                
            if not data_received:
                print("    No data received.")
                
            ser.close()
            
        except serial.SerialException as e:
            print(f"    Failed to open port: {e}")
        except Exception as e:
            print(f"    Error: {e}")

if __name__ == "__main__":
    scan_baud_rates()
