import sys

def read_hid():
    print("HID RFID Reader Test")
    print("--------------------")
    print("Click inside this terminal window.")
    print("Scan your RFID tag now.")
    print("If the reader is in HID mode, the tag ID should appear below.")
    print("Press Ctrl+C to exit.")
    print("--------------------")

    while True:
        try:
            # simply read a line from stdin
            data = input("Waiting for input (Scan tag now): ")
            print(f"Received: {data}")
            try:
                print(f"Hex: {data.encode('utf-8').hex()}")
            except:
                pass
            print("-" * 20)
        except KeyboardInterrupt:
            print("\nExiting...")
            break
        except EOFError:
            break
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    read_hid()
