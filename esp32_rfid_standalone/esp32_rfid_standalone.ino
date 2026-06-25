/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/*
 * ESP32 RFID STANDALONE READER
 * Works WITHOUT laptop - reads RFID and stores/displays data
 * Ready for WiFi transmission to phone app
 * 
 * WIRING:
 * ==========================================
 * RFID Module → ESP32
 * ==========================================
 * RFID TX     → GPIO 16 (RX2)
 * RFID RX     → GPIO 17 (TX2)  
 * RFID GND    → GND
 * RFID VCC    → 3.3V (NOT 5V!)
 * ==========================================
 * 
 * BUILT-IN LED will BLINK when tag is detected!
 */

#include <HardwareSerial.h>

// === CONFIGURATION ===
const int RFID_RX_PIN = 16;      // RFID TX connects here
const int RFID_TX_PIN = 17;      // RFID RX connects here
const int RFID_BAUD = 38400;     // Your RFID module's baud rate
const int LED_PIN = 2;           // Built-in LED (most ESP32 boards)

HardwareSerial rfidSerial(2);

// === VARIABLES ===
int tagCount = 0;
String lastTagHex = "";
unsigned long lastTagTime = 0;

void setup() {
  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Setup USB Serial (for debugging when laptop connected)
  Serial.begin(115200);
  delay(500);
  
  // Startup blink pattern (3 fast blinks = system ready)
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  ESP32 RFID STANDALONE READER v1.0   ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println("\n📡 Initializing RFID reader...");
  Serial.print("   Baud: "); Serial.println(RFID_BAUD);
  Serial.print("   RX Pin: GPIO "); Serial.println(RFID_RX_PIN);
  Serial.print("   TX Pin: GPIO "); Serial.println(RFID_TX_PIN);
  
  // Initialize RFID Serial
  rfidSerial.begin(RFID_BAUD, SERIAL_8N1, RFID_RX_PIN, RFID_TX_PIN);
  delay(100);
  
  Serial.println("\n✅ READY! Waiting for RFID tags...");
  Serial.println("   LED will BLINK when tag detected\n");
  Serial.println("════════════════════════════════════════\n");
}

void loop() {
  // Check for RFID data
  if (rfidSerial.available()) {
    
    // LED ON - reading data
    digitalWrite(LED_PIN, HIGH);
    
    String hexData = "";
    int byteCount = 0;
    
    // Read all available bytes
    while (rfidSerial.available()) {
      byte b = rfidSerial.read();
      
      // Build hex string
      if (b < 0x10) hexData += "0";
      hexData += String(b, HEX);
      hexData += " ";
      
      byteCount++;
      delay(1);  // Small delay to catch all bytes
    }
    
    hexData.trim();  // Remove trailing space
    hexData.toUpperCase();
    
    // Only process if we got meaningful data (more than 2 bytes)
    if (byteCount > 2) {
      tagCount++;
      lastTagHex = hexData;
      lastTagTime = millis();
      
      // Print to Serial (if laptop connected)
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("🏷️  TAG #"); Serial.print(tagCount);
      Serial.print(" | Time: "); Serial.print(millis() / 1000); Serial.println("s");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("HEX: "); Serial.println(hexData);
      Serial.print("Bytes: "); Serial.println(byteCount);
      Serial.println();
      
      // BLINK LED 2 times = tag detected!
      for(int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
      }
    }
    
    // LED OFF
    digitalWrite(LED_PIN, LOW);
  }
  
  // Small LED pulse every 5 seconds to show system is alive
  if (millis() % 5000 < 50) {
    digitalWrite(LED_PIN, HIGH);
    delay(30);
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(10);
}

// === HELPER FUNCTIONS (for WiFi integration later) ===

// This function will send tag data to phone via WiFi
// Currently just returns the data - you'll add WiFi code here
String getLastTagData() {
  return lastTagHex;
}

int getTagCount() {
  return tagCount;
}
