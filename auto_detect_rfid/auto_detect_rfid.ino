/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

#include <HardwareSerial.h>

// Pins
const int RX_PIN = 16;
const int TX_PIN = 17;

struct Config {
  long baud;
  uint32_t config;
  bool invert;
  String name;
};

// Common RFID configs
Config configs[] = {
  {115200, SERIAL_8N2, false, "115200 8N2 Normal"},
  {115200, SERIAL_8N2, true,  "115200 8N2 INVERTED"},
  {115200, SERIAL_8N1, false, "115200 8N1 Normal"},
  {115200, SERIAL_8N1, true,  "115200 8N1 INVERTED"},
  
  {9600, SERIAL_8N1, false,   "9600 8N1 Normal"},
  {9600, SERIAL_8N1, true,    "9600 8N1 INVERTED"},
  
  {38400, SERIAL_8N1, false,  "38400 8N1 Normal"},
  {38400, SERIAL_8N1, true,   "38400 8N1 INVERTED"},
  
  {57600, SERIAL_8N1, false,  "57600 8N1 Normal"},
  {57600, SERIAL_8N1, true,   "57600 8N1 INVERTED"}
};

int currentConfig = 0;
unsigned long stateStart = 0;
const unsigned long TIMEOUT = 3000; // 3 seconds per config
bool locked = false;

HardwareSerial rfidSerial(2);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== RFID SMART AUTO-DETECT ===");
  Serial.println("HOLD TAG NEAR READER NOW!");
  
  startConfig(0);
}

void startConfig(int index) {
  currentConfig = index;
  rfidSerial.end();
  delay(100);
  rfidSerial.begin(configs[currentConfig].baud, configs[currentConfig].config, RX_PIN, TX_PIN, configs[currentConfig].invert);
  
  Serial.print("TESTING: ");
  Serial.println(configs[currentConfig].name);
  stateStart = millis();
}

int validCharCount = 0;
String maybeTag = "";

void loop() {
  if (locked) {
    if (rfidSerial.available()) {
      Serial.write(rfidSerial.read());
    }
    return;
  }

  // Auto switch
  if (millis() - stateStart > TIMEOUT) {
    int next = (currentConfig + 1) % (sizeof(configs)/sizeof(configs[0]));
    startConfig(next);
    validCharCount = 0;
    maybeTag = "";
  }

  while (rfidSerial.available()) {
    int b = rfidSerial.read();
    char c = (char)b;

    // Check for readable ASCII (0-9, A-Z, a-z)
    bool isAlphaNum = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    
    if (isAlphaNum) {
      validCharCount++;
      maybeTag += c;
    } else {
      // If we hit a newline or space after getting some valid chars, that's a good sign
      if (validCharCount > 3 && (c == '\n' || c == '\r' || c == ' ')) {
          locked = true;
          Serial.println("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          Serial.println("!!! LOCKED ON CONFIG FOUND !!!");
          Serial.print("!!! MODE: "); Serial.println(configs[currentConfig].name);
          Serial.print("!!! DATA: "); Serial.println(maybeTag);
          Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          return;
      }
      // Reset if we saw garbage but hadn't locked yet
      if (validCharCount < 3) {
        validCharCount = 0;
        maybeTag = "";
      }
    }
    
    // Debug print (minimal)
    // Serial.print(c); 
  }
}
