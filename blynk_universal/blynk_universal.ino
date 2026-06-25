/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 → Blynk 38400 Reader
 * Config: 38400 8N1 NORMAL (Detected via Auto-Scan)
 ************************************************************/

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID   "TMPL3ajs_Zavb"
#define BLYNK_TEMPLATE_NAME "ESP32RFID"
#define BLYNK_AUTH_TOKEN    "Pik0pCvfvUu8X4-wQTGh35Rutgzo1t7z"

char ssid[] = "CMF";
char pass[] = "123456789";

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const int RX_PIN = 16;
const int TX_PIN = 17;
HardwareSerial rfidSerial(2);

// Buffer
String hexString = "";
unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("=== ESP32 38400 Reader ===");

  // LOCKING TO DETECTED CONFIG: 38400 8N1 Normal
  rfidSerial.begin(38400, SERIAL_8N1, RX_PIN, TX_PIN, false); 
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.virtualWrite(V0, "Ready (38400bps)");
}

void loop() {
  Blynk.run();
  
  if (rfidSerial.available()) {
    while (rfidSerial.available()) {
      int b = rfidSerial.read();
      if (b < 0x10) hexString += "0";
      hexString += String(b, HEX);
      hexString += " ";
    }
    
    // Send to App (Rate Limited)
    if (millis() - lastSend > 500) {
      Serial.println("TAG: " + hexString);
      Blynk.virtualWrite(V0, "Hex: " + hexString);
      hexString = ""; // Reset
      lastSend = millis();
    }
  }
}
