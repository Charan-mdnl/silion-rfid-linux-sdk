/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 STANDALONE RFID → BLYNK (NO LAPTOP NEEDED!)
 * 
 * PRODUCTION USE (Industrial Customers):
 * - Connect RFID module to ESP32 via UART
 * - Power on ESP32
 * - Tags automatically appear on phone app!
 * 
 * DEVELOPMENT/TESTING (For You):
 * - Connect ESP32 to laptop via USB
 * - Open Serial Monitor to verify tags reading correctly
 * 
 * WIRING:
 * RFID TX  → ESP32 GPIO 16 (RX2)
 * RFID RX  → ESP32 GPIO 17 (TX2)
 * RFID GND → ESP32 GND
 * RFID VCC → ESP32 3.3V
 ************************************************************/

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID   "TMPL3ajs_Zavb"
#define BLYNK_TEMPLATE_NAME "ESP32RFID"
#define BLYNK_AUTH_TOKEN    "Pik0pCvfvUu8X4-wQTGh35Rutgzo1t7z"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HardwareSerial.h>

// WiFi credentials (customer configuration)
char ssid[] = "CMF";
char pass[] = "123456789";

// RFID Configuration
const int RFID_RX_PIN = 16;
const int RFID_TX_PIN = 17;
const int RFID_BAUD = 38400;
const int LED_PIN = 2;

HardwareSerial rfidSerial(2);

// Variables
int tagCount = 0;
String lastTagHex = "";
unsigned long lastTagTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Serial Monitor (optional - only for development verification)
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔═════════════════════════════════════════════╗");
  Serial.println("║  ESP32 STANDALONE RFID → BLYNK v2.0        ║");
  Serial.println("╚═════════════════════════════════════════════╝\n");
  Serial.println("🏭 PLUG-AND-PLAY MODE");
  Serial.println("   (No laptop needed in production)\n");
  
  // Initialize RFID Serial
  Serial.println("📡 Initializing RFID...");
  rfidSerial.begin(RFID_BAUD, SERIAL_8N1, RFID_RX_PIN, RFID_TX_PIN);
  Serial.println("✅ RFID initialized at 38400 baud");
  Serial.println();
  
  // Connect to WiFi
  Serial.print("📶 Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    // LED blink while connecting
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi Connected!");
    Serial.print("📍 IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("❌ WiFi FAILED! Check credentials.");
    // Continue anyway - RFID will still work locally
  }
  Serial.println();
  
  // Connect to Blynk
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("📱 Connecting to Blynk...");
    Blynk.config(BLYNK_AUTH_TOKEN);
    
    if (Blynk.connect()) {
      Serial.println("✅ Blynk Connected!");
      Blynk.virtualWrite(V0, "ESP32 RFID Reader Online");
    } else {
      Serial.println("⚠️  Blynk connection failed (will retry)");
    }
  }
  
  Serial.println();
  Serial.println("════════════════════════════════════════════");
  Serial.println("✅ SYSTEM READY - SCANNING FOR TAGS...");
  Serial.println("════════════════════════════════════════════\n");
  
  // Startup LED pattern
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop() {
  Blynk.run();
  
  // Check for RFID data
  if (rfidSerial.available()) {
    digitalWrite(LED_PIN, HIGH);
    
    String hexData = "";
    int byteCount = 0;
    unsigned long startTime = millis();
    
    // Read all available bytes (with timeout)
    while (rfidSerial.available() && (millis() - startTime < 100)) {
      byte b = rfidSerial.read();
      
      if (b < 0x10) hexData += "0";
      hexData += String(b, HEX);
      hexData += " ";
      
      byteCount++;
      delay(2);
    }
    
    hexData.trim();
    hexData.toUpperCase();
    
    // Process if meaningful data (at least 4 bytes)
    // and not duplicate (debounce)
    unsigned long currentTime = millis();
    if (byteCount >= 4 && (currentTime - lastTagTime > 500)) {
      tagCount++;
      lastTagHex = hexData;
      lastTagTime = currentTime;
      
      // ===== SERIAL MONITOR OUTPUT (for development verification) =====
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("🏷️  TAG #");
      Serial.print(tagCount);
      Serial.print(" | ");
      Serial.print(currentTime / 1000);
      Serial.println("s");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.print("EPC: ");
      Serial.println(hexData);
      Serial.print("Bytes: ");
      Serial.println(byteCount);
      
      // ===== SEND TO BLYNK PHONE APP =====
      if (Blynk.connected()) {
        String phoneMessage = "Tag #" + String(tagCount) + ": " + hexData;
        Blynk.virtualWrite(V0, phoneMessage);
        Serial.println("📱 Sent to Blynk");
      } else {
        Serial.println("⚠️  Blynk offline (will retry)");
      }
      
      Serial.println();
      
      // LED blink pattern (3 fast blinks)
      for(int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(80);
        digitalWrite(LED_PIN, HIGH);
        delay(80);
      }
    }
    
    digitalWrite(LED_PIN, LOW);
  }
  
  // Auto-reconnect to Blynk if needed
  if (WiFi.status() == WL_CONNECTED && !Blynk.connected()) {
    static unsigned long lastReconnect = 0;
    if (millis() - lastReconnect > 30000) {
      Serial.println("🔄 Reconnecting to Blynk...");
      if (Blynk.connect()) {
        Serial.println("✅ Reconnected!");
      }
      lastReconnect = millis();
    }
  }
  
  delay(10);
}
