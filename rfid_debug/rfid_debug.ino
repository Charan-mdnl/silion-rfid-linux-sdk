/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 RFID DEBUG VERSION
 * Shows detailed debug info to help find the problem
 * 
 * WIRING CHECK:
 * RFID TX  → ESP32 GPIO 16 (RX2)
 * RFID RX  → ESP32 GPIO 17 (TX2)
 * RFID GND → ESP32 GND
 * RFID VCC → ESP32 3.3V (NOT 5V!)
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
const int RFID_RX_PIN = 16;
const int RFID_TX_PIN = 17;
const int RFID_BAUD = 38400;
const int LED_PIN = 2;

HardwareSerial rfidSerial(2);

int tagCount = 0;
int loopCount = 0;
int bytesSeen = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(115200);
  delay(2000);  // Longer delay to ensure Serial is ready
  
  Serial.println("\n\n\n");
  Serial.println("╔═════════════════════════════════════════════╗");
  Serial.println("║  ESP32 RFID DEBUG MODE v1.0                ║");
  Serial.println("╚═════════════════════════════════════════════╝\n");
  
  // TEST 1: Initialize RFID
  Serial.println("TEST 1: Initializing RFID Serial...");
  rfidSerial.begin(RFID_BAUD, SERIAL_8N1, RFID_RX_PIN, RFID_TX_PIN);
  delay(200);
  Serial.println("✅ RFID Serial initialized");
  Serial.print("   Baud: "); Serial.println(RFID_BAUD);
  Serial.print("   RX Pin: GPIO "); Serial.println(RFID_RX_PIN);
  Serial.print("   TX Pin: GPIO "); Serial.println(RFID_TX_PIN);
  Serial.println();
  
  // TEST 2: WiFi Connection
  Serial.println("TEST 2: Connecting to WiFi...");
  Serial.print("   SSID: "); Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("   IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi FAILED!");
    Serial.println("   Continuing without WiFi...");
  }
  Serial.println();
  
  // TEST 3: Blynk Connection (only if WiFi works)
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("TEST 3: Connecting to Blynk...");
    Blynk.config(BLYNK_AUTH_TOKEN);
    if (Blynk.connect()) {
      Serial.println("✅ Blynk Connected!");
      Blynk.virtualWrite(V0, "ESP32 Debug Mode Active");
    } else {
      Serial.println("❌ Blynk connection failed");
      Serial.println("   Continuing in standalone mode...");
    }
  } else {
    Serial.println("TEST 3: Skipping Blynk (no WiFi)");
  }
  Serial.println();
  
  // TEST 4: RFID Port Test
  Serial.println("TEST 4: Testing RFID port...");
  Serial.println("   Waiting 3 seconds for data...");
  
  unsigned long testStart = millis();
  bool gotData = false;
  while (millis() - testStart < 3000) {
    if (rfidSerial.available()) {
      gotData = true;
      Serial.println("   ✅ RFID port is receiving data!");
      break;
    }
    delay(100);
  }
  
  if (!gotData) {
    Serial.println("   ❌ NO DATA on RFID port!");
    Serial.println("   Possible issues:");
    Serial.println("      - RFID module not connected");
    Serial.println("      - Wrong TX/RX pins");
    Serial.println("      - RFID module not powered");
    Serial.println("      - Wrong baud rate");
  }
  Serial.println();
  
  Serial.println("════════════════════════════════════════════");
  Serial.println("DEBUG MODE ACTIVE");
  Serial.println("Hold tag near RFID antenna now!");
  Serial.println("════════════════════════════════════════════\n");
  
  // Startup blink
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop() {
  Blynk.run();
  loopCount++;
  
  // Status update every 5 seconds
  if (loopCount % 500 == 0) {
    Serial.print("[STATUS] Loop: ");
    Serial.print(loopCount);
    Serial.print(" | Bytes seen: ");
    Serial.print(bytesSeen);
    Serial.print(" | Tags: ");
    Serial.println(tagCount);
  }
  
  // Check for RFID data
  if (rfidSerial.available()) {
    digitalWrite(LED_PIN, HIGH);
    
    Serial.println("\n>>> RFID DATA DETECTED! <<<");
    Serial.print("Raw bytes available: ");
    Serial.println(rfidSerial.available());
    
    String hexData = "";
    int byteCount = 0;
    
    // Read with detailed output
    while (rfidSerial.available()) {
      byte b = rfidSerial.read();
      bytesSeen++;
      
      if (b < 0x10) hexData += "0";
      hexData += String(b, HEX);
      hexData += " ";
      
      byteCount++;
      
      // Show first 10 bytes immediately for debug
      if (byteCount <= 10) {
        Serial.print("  Byte ");
        Serial.print(byteCount);
        Serial.print(": 0x");
        if (b < 0x10) Serial.print("0");
        Serial.print(b, HEX);
        Serial.print(" (");
        Serial.print(b);
        Serial.println(")");
      }
      
      delay(2);
    }
    
    hexData.trim();
    hexData.toUpperCase();
    
    Serial.println("\n--- COMPLETE DATA ---");
    Serial.print("Total bytes: ");
    Serial.println(byteCount);
    Serial.print("HEX: ");
    Serial.println(hexData);
    
    if (byteCount >= 4) {
      tagCount++;
      
      Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("✅ VALID TAG #");
      Serial.println(tagCount);
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("TAG ID: ");
      Serial.println(hexData);
      Serial.println();
      
      String phoneMessage = "Tag #" + String(tagCount) + ": " + hexData;
      
      if (Blynk.connected()) {
        Blynk.virtualWrite(V0, phoneMessage);
        Serial.println("📱 Sent to Blynk!");
      } else {
        Serial.println("⚠️  Not sent (Blynk disconnected)");
      }
      
      // Blink
      for(int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(80);
        digitalWrite(LED_PIN, HIGH);
        delay(80);
      }
    } else {
      Serial.println("⚠️  Too few bytes, ignoring");
    }
    
    digitalWrite(LED_PIN, LOW);
    Serial.println();
  }
  
  delay(10);
}
