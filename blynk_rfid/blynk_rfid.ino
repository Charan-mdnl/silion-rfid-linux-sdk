/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 → Blynk IoT
 * Reads RFID from hardware UART (Serial2) and sends to Blynk (V0)
 * Diagnostic printing to USB Serial.
 ************************************************************/

#define BLYNK_PRINT Serial

// Blynk values (from user)
#define BLYNK_TEMPLATE_ID   "TMPL3ajs_Zavb"
#define BLYNK_TEMPLATE_NAME "ESP32RFID"
#define BLYNK_AUTH_TOKEN    "Pik0pCvfvUu8X4-wQTGh35Rutgzo1t7z"

// WiFi credentials
char ssid[] = "CMF";
char pass[] = "123456789";

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const int RFID_RX_PIN = 16; // RFID TX -> ESP32 RX2
const int RFID_TX_PIN = 17; // ESP32 TX2 -> RFID RX (only if sending commands)
HardwareSerial rfidSerial(2);

const unsigned long RFID_BAUD = 115200;
const uint32_t RFID_CONFIG = SERIAL_8N2; 
bool RFID_INVERT = true; // Set to true because we saw [00] (Idle Low) artifacts

String rfidBuffer = "";
unsigned long lastReceivedAt = 0;
const unsigned long INACTIVITY_MS = 2000;
const bool PRINT_HEX_DIAG = false;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("ESP32 RFID -> Serial + Blynk starting...");

  // rfidSerial.begin(baud, config, rx, tx, invert)
  rfidSerial.begin(RFID_BAUD, RFID_CONFIG, RFID_RX_PIN, RFID_TX_PIN, RFID_INVERT);
  delay(50);
  Serial.print("rfidSerial started on RX=");
  Serial.print(RFID_RX_PIN);
  Serial.print(" TX=");
  Serial.print(RFID_TX_PIN);
  Serial.print(" @ ");
  Serial.print(RFID_BAUD);
  Serial.print(" config=");
  Serial.print(RFID_CONFIG);
  Serial.print(" invert=");
  Serial.println(RFID_INVERT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Called Blynk.begin()");
}

void loop() {
  Blynk.run();

  while (rfidSerial.available()) {
    int b = rfidSerial.read();
    lastReceivedAt = millis();

    if (PRINT_HEX_DIAG) {
      if (b < 0x10) Serial.print('0');
      Serial.print(b, HEX);
      Serial.print(' ');
    } else {
      char c = (char)b;
      if (c == '\r') {
        // ignore
      } else if (c == '\n') {
        if (rfidBuffer.length() > 0) {
          Serial.print("[RFID] ");
          Serial.println(rfidBuffer);
          if (Blynk.connected()) Blynk.virtualWrite(V0, rfidBuffer);
          rfidBuffer = "";
        }
      } else {
        rfidBuffer += c;
        if (rfidBuffer.length() > 512) rfidBuffer = rfidBuffer.substring(rfidBuffer.length()-512);
      }
    }
  }

  if (PRINT_HEX_DIAG && (millis() - lastReceivedAt > 100) && lastReceivedAt != 0) {
    Serial.println();
    lastReceivedAt = 0;
  }

  if (!PRINT_HEX_DIAG && rfidBuffer.length() > 0 && (millis() - lastReceivedAt > INACTIVITY_MS)) {
    Serial.print("[RFID - timeout] partial buffer: ");
    Serial.println(rfidBuffer);
    if (Blynk.connected()) Blynk.virtualWrite(V0, rfidBuffer);
    rfidBuffer = "";
  }

  delay(10);
}
