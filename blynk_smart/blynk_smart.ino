/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 → Blynk Smart Reader (Universal)
 * Config: 115200 8N2 INVERTED (Hardcoded)
 * Features:
 *  - Crash Proof (Rate Limited)
 *  - Dual Mode (ASCII Text + Binary Hex)
 ************************************************************/

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID   "TMPL3ajs_Zavb"
#define BLYNK_TEMPLATE_NAME "ESP32RFID"
#define BLYNK_AUTH_TOKEN    "Pik0pCvfvUu8X4-wQTGh35Rutgzo1t7z"

char ssid[] = "CMF";
char pass[] = "123456789";

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// --- HARDWARE CONFIG ---
const int RX_PIN = 16;
const int TX_PIN = 17;
HardwareSerial rfidSerial(2);

// Buffers
String asciiBuffer = "";
String hexBuffer = "";
unsigned long lastDataTime = 0;
unsigned long lastBlynkSend = 0;
const int SEND_INTERVAL = 1000; // Limit Blynk Updates (1 sec)

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("=== ESP32 Smart Reader ===");

  // 115200 8N2 INVERTED is the strongest candidate
  rfidSerial.begin(115200, SERIAL_8N2, RX_PIN, TX_PIN, true);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.virtualWrite(V0, "Ready. Scan Tag.");
}

void loop() {
  Blynk.run();
  
  // Read Data
  while (rfidSerial.available()) {
    int b = rfidSerial.read();
    char c = (char)b;
    lastDataTime = millis();

    // 1. Build Hex String (for debug/binary tags)
    // Filter noise like [00] or [FF] if occurring in bursts
    if (b != 0x00 && b != 0xFF) {
      if (b < 0x10) hexBuffer += "0";
      hexBuffer += String(b, HEX);
    }

    // 2. Build ASCII String (for "epc:..." tags)
    if (c >= 32 && c <= 126) {
      asciiBuffer += c;
    } else if (c == '\n' || c == '\r') {
      // End of ASCII Line
      if (asciiBuffer.length() > 5) {
        processASCII(asciiBuffer);
        asciiBuffer = "";
        hexBuffer = ""; // Clear hex too if we found meaningful ASCII
      }
    }
    
    yield(); // Watchdog safety
  }

  // Timeout: If silence for 200ms and we have Hex data (but no ASCII newline)
  if (hexBuffer.length() > 0 && millis() - lastDataTime > 200) {
    // Only send if we haven't sent recently
    if (millis() - lastBlynkSend > SEND_INTERVAL) {
        String output = "Hex: " + hexBuffer;
        Serial.println(output);
        Blynk.virtualWrite(V0, output);
        lastBlynkSend = millis();
    }
    hexBuffer = "";
    asciiBuffer = "";
  }
}

void processASCII(String line) {
  Serial.println("ASCII: " + line);
  
  // Rate Limit
  if (millis() - lastBlynkSend > SEND_INTERVAL) {
    // Clean Display
    if (line.indexOf("epc") >= 0) {
       Blynk.virtualWrite(V0, line); // Show full line like "epc: E2..."
    } else {
       Blynk.virtualWrite(V0, "Tag: " + line);
    }
    lastBlynkSend = millis();
  }
}
