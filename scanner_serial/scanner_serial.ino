/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

#include <HardwareSerial.h>

// Pins
const int RX_PIN = 16;
const int TX_PIN = 17;

// Configs to test
struct Config {
  long baud;
  uint32_t config;
  bool invert;
  const char* name;
};

Config configs[] = {
  {115200, SERIAL_8N2, false, "115200 8N2 (Normal)"},
  {115200, SERIAL_8N2, true,  "115200 8N2 (INVERTED)"}, // Try Inverted
  {9600,   SERIAL_8N1, false, "9600 8N1 (Normal)"},
  {9600,   SERIAL_8N1, true,  "9600 8N1 (INVERTED)"},   // Try Inverted
  {115200, SERIAL_8N1, false, "115200 8N1 (Normal)"},
  {115200, SERIAL_8N1, true,  "115200 8N1 (INVERTED)"},
  {38400,  SERIAL_8N1, false, "38400 8N1 (Normal)"},
  {38400,  SERIAL_8N1, true,  "38400 8N1 (INVERTED)"}
};

int currentConfig = 0;
unsigned long lastSwitchTime = 0;
const unsigned long SWITCH_DELAY = 10000; // 10 seconds per config

HardwareSerial rfidSerial(2);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== RFID Serial Scanner (Invert Support) ===");
  Serial.println("Connect RFID to GPIO 16 (RX) and 17 (TX).");
  Serial.println("Present a tag constantly. I will switch modes every 10 seconds.");
  
  startConfig(0);
}

void startConfig(int index) {
  currentConfig = index;
  rfidSerial.end();
  delay(100);
  // begin(baud, config, rx, tx, invert)
  rfidSerial.begin(configs[currentConfig].baud, configs[currentConfig].config, RX_PIN, TX_PIN, configs[currentConfig].invert);
  
  Serial.println("\n------------------------------------------------");
  Serial.print("SWITCHING TO: ");
  Serial.println(configs[currentConfig].name);
  Serial.println("------------------------------------------------");
  lastSwitchTime = millis();
}

void loop() {
  // auto switch
  if (millis() - lastSwitchTime > SWITCH_DELAY) {
    int next = (currentConfig + 1) % (sizeof(configs)/sizeof(configs[0]));
    startConfig(next);
  }

  // read and print
  while (rfidSerial.available()) {
    int b = rfidSerial.read();
    
    // Print in Hex
    Serial.print("[");
    if (b < 0x10) Serial.print("0");
    Serial.print(b, HEX);
    Serial.print("]");
    
    // If printable ASCII, print it too
    if (b >= 32 && b <= 126) {
      Serial.print((char)b);
    }
    Serial.print(" ");
  }
  
  // minimal delay
  delay(1);
}
