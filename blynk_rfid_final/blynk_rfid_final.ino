/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 RFID → Blynk Phone App
 * Reads RFID tags and sends to phone via WiFi
 * Also displays in Serial Monitor
 * 
 * WIRING:
 * RFID TX  → ESP32 GPIO 16 (RX2)
 * RFID RX  → ESP32 GPIO 17 (TX2)
 * RFID GND → ESP32 GND
 * RFID VCC → ESP32 3.3V
 ************************************************************/

#define BLYNK_PRINT Serial

// Blynk Configuration
#define BLYNK_TEMPLATE_ID   "TMPL3ajs_Zavb"
#define BLYNK_TEMPLATE_NAME "ESP32RFID"
#define BLYNK_AUTH_TOKEN    "Pik0pCvfvUu8X4-wQTGh35Rutgzo1t7z"

// WiFi credentials
char ssid[] = "CMF";
char pass[] = "123456789";

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HardwareSerial.h>

// === RFID Configuration ===
const int RFID_RX_PIN = 16;      // RFID TX connects here
const int RFID_TX_PIN = 17;      // RFID RX connects here
const int RFID_BAUD = 38400;     // Your module's baud rate
const int LED_PIN = 2;           // Built-in LED

HardwareSerial rfidSerial(2);

// === Variables ===
int tagCount = 0;
String lastTagHex = "";
unsigned long lastTagTime = 0;
unsigned long lastBlinkTime = 0;

void setup() {
  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Setup Serial Monitor
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔═════════════════════════════════════════╗");
  Serial.println("║  ESP32 RFID → BLYNK READER v1.0        ║");
  Serial.println("╚═════════════════════════════════════════╝\n");
  
  // Initialize RFID
  rfidSerial.begin(RFID_BAUD, SERIAL_8N1, RFID_RX_PIN, RFID_TX_PIN);
  Serial.println("📡 RFID initialized (38400 baud, GPIO 16/17)");
  
  // Connect to Blynk
  Serial.print("📱 Connecting to WiFi: ");
  Serial.println(ssid);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  Serial.println("✅ Connected to Blynk!");
  Serial.println("✅ Ready to scan RFID tags\n");
  Serial.println("════════════════════════════════════════\n");
  
  // Startup blink (3 fast blinks)
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  
  Blynk.virtualWrite(V0, "ESP32 RFID Ready");
}

void loop() {
  Blynk.run();
  
  // Check for RFID data
  if (rfidSerial.available()) {
    
    // Turn LED ON while reading
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
      delay(2);  // Small delay to catch all bytes
    }
    
    hexData.trim();
    hexData.toUpperCase();
    
    // Only process if we got meaningful data (at least 4 bytes)
    // Based on yesterday's readings: 70 80 07 C0 89 F3, 70 CE 1E F4, etc.
    if (byteCount >= 4) {
      tagCount++;
      lastTagHex = hexData;
      lastTagTime = millis();
      
      // ===== SERIAL MONITOR OUTPUT =====
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("🏷️  RFID TAG #");
      Serial.print(tagCount);
      Serial.print(" | Time: ");
      Serial.print(millis() / 1000);
      Serial.println("s");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("TAG ID: ");
      Serial.println(hexData);
      Serial.print("Bytes: ");
      Serial.println(byteCount);
      Serial.println();
      
      // ===== SEND TO PHONE APP =====
      String phoneMessage = "Tag #" + String(tagCount) + ": " + hexData;
      Blynk.virtualWrite(V0, phoneMessage);
      
      Serial.println("📱 Sent to Phone: " + phoneMessage);
      Serial.println();
      
      // Blink LED 3 times = TAG DETECTED!
      for(int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(80);
        digitalWrite(LED_PIN, HIGH);
        delay(80);
      }
    }
    
    // Turn LED OFF
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(10);
}
