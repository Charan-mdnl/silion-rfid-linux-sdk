/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/*
 * Simple RFID Display - No Libraries Needed!
 * Reads from RFID module at 38400 baud and displays on Serial Monitor
 * 
 * CONNECTIONS (as you've already done):
 * RFID TX  --> ESP32 GPIO 16
 * RFID RX  --> ESP32 GPIO 17
 * RFID GND --> ESP32 GND
 * RFID VCC --> ESP32 3.3V
 */

#include <HardwareSerial.h>

// RFID Serial (Serial2)
const int RX_PIN = 16;  // RFID TX connects here
const int TX_PIN = 17;  // RFID RX connects here
const int RFID_BAUD = 38400;  // Detected baud rate

HardwareSerial rfidSerial(2);

int tagCount = 0;

void setup() {
  // USB Serial for display (Serial Monitor)
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════════════╗");
  Serial.println("║        RFID TAG READER - LIVE MONITOR         ║");
  Serial.println("╚════════════════════════════════════════════════╝");
  Serial.println();
  Serial.println("📌 Configuration:");
  Serial.println("   RFID Baud: 38400");
  Serial.println("   RX Pin: GPIO 16");
  Serial.println("   TX Pin: GPIO 17");
  Serial.println();
  Serial.println("🏷️  HOLD YOUR RFID TAG NEAR THE READER!");
  Serial.println("================================================");
  Serial.println();
  
  // Initialize RFID Serial
  rfidSerial.begin(RFID_BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(100);
  
  Serial.println("✅ RFID Reader initialized. Listening...\n");
}

void loop() {
  // Check if data is available from RFID module
  if (rfidSerial.available()) {
    tagCount++;
    
    // Get timestamp
    unsigned long timestamp = millis() / 1000;
    
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.print("🏷️  Tag #");
    Serial.print(tagCount);
    Serial.print(" | Time: ");
    Serial.print(timestamp);
    Serial.println("s");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    
    // Read all available bytes
    Serial.print("HEX: ");
    int byteCount = 0;
    
    while (rfidSerial.available()) {
      byte b = rfidSerial.read();
      
      // Print in HEX format
      if (b < 0x10) Serial.print("0");
      Serial.print(b, HEX);
      Serial.print(" ");
      
      byteCount++;
      delay(1);  // Small delay to catch all bytes
    }
    
    Serial.println();
    Serial.print("Length: ");
    Serial.print(byteCount);
    Serial.println(" bytes");
    Serial.println();
  }
  
  delay(10);  // Small delay
}
