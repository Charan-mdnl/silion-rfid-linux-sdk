/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

#include <HardwareSerial.h>

// --- CONFIGURATION ---
// User Connection: ESP32 <--> RS232-TTL <--> RFID Reader
#define RX_PIN 16
#define TX_PIN 17
#define BAUD_RATE 115200

HardwareSerial rfidSerial(2);

// Standard "Inventory" command captured from working Linux driver
// FF 05 22 00 00 03 00 C8 38 14
// FF: Header
// 05: Length
// 22: Command (Inventory?)
// ...
// C8: Timeout (200ms)
// 38 14: Checksum
const byte CMD_INVENTORY[] = {0xFF, 0x05, 0x22, 0x00, 0x00, 0x03, 0x00, 0xC8, 0x38, 0x14};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP32 RFID Raw Protocol Tester ===");
  Serial.println("Attempting to connect to reader...");

  // Initialize RFID Serial (Serial2)
  rfidSerial.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(100);

  // Send initial wakeup/magic bytes seen in trace (optional, but safe)
  // ff 0e aa 4d 6f 64 75 6c 65 74 65 63 68 aa 49 f3 bb 03 91
  byte wakeUp[] = {0xFF, 0x0E, 0xAA, 0x4D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x74, 0x65, 0x63, 0x68, 0xAA, 0x49, 0xF3, 0xBB, 0x03, 0x91};
  rfidSerial.write(wakeUp, sizeof(wakeUp));
  delay(200);
  
  Serial.println("Initialization sent. Starting scan loop...");
}

void loop() {
  // 1. Send Inventory Command
  rfidSerial.write(CMD_INVENTORY, sizeof(CMD_INVENTORY));
  
  // 2. Listen for Response
  unsigned long startTime = millis();
  bool receivedData = false;
  
  while (millis() - startTime < 300) { // Wait up to 300ms (command has 200ms timeout)
    if (rfidSerial.available()) {
      receivedData = true;
      Serial.print("RX: ");
      while (rfidSerial.available()) {
        byte b = rfidSerial.read();
        if (b < 0x10) Serial.print("0");
        Serial.print(b, HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
  }

  if (!receivedData) {
     // Serial.println("No response...");
  }

  delay(500); // Scan every 500ms
}
