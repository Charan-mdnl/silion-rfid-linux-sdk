/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 → Blynk Scanner
 * Cycles Serial Configurations and sends output to Blynk App
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

struct Config {
  long baud;
  uint32_t config;
  bool invert;
  String name;
};

// Test List based on common RFID readers + Inverted logic hypothesis
Config configs[] = {
  {115200, SERIAL_8N2, true,  "115200 8N2 INV"}, // Best guess
  {115200, SERIAL_8N1, true,  "115200 8N1 INV"},
  {38400,  SERIAL_8N1, true,  "38400 8N1 INV"},
  {9600,   SERIAL_8N1, true,  "9600 8N1 INV"},
  {57600,  SERIAL_8N1, true,  "57600 8N1 INV"},
  {115200, SERIAL_8N1, false, "115200 8N1 NOR"}, // Standard
  {9600,   SERIAL_8N1, false, "9600 8N1 NOR"}
};

int currentConfig = 0;
unsigned long lastSwitch = 0;
const int SWITCH_DELAY = 4000; // 4 seconds per mode

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Start first config
  applyConfig(0);
}

void applyConfig(int index) {
  currentConfig = index;
  rfidSerial.end();
  delay(100);
  rfidSerial.begin(configs[index].baud, configs[index].config, RX_PIN, TX_PIN, configs[index].invert);
  
  String msg = "TRY: " + configs[index].name;
  Serial.println(msg);
  Blynk.virtualWrite(V0, msg); // Show current mode in App
}

void loop() {
  Blynk.run();

  // Auto-Switch Logic
  if (millis() - lastSwitch > SWITCH_DELAY) {
    lastSwitch = millis();
    int next = (currentConfig + 1) % (sizeof(configs)/sizeof(configs[0]));
    applyConfig(next);
  }

  // Read Data
  String chunk = "";
  while (rfidSerial.available()) {
    int b = rfidSerial.read();
    
    // Convert to readable hex/ascii mix
    if (b >= 32 && b <= 126) {
      chunk += (char)b;
    } else {
      chunk += "[";
      if (b < 0x10) chunk += "0";
      chunk += String(b, HEX);
      chunk += "]";
    }
  }

  // Send Data to App if any
  if (chunk.length() > 0) {
    Serial.println("DATA: " + chunk); 
    Blynk.virtualWrite(V0, chunk); // Send immediately
  }
}
