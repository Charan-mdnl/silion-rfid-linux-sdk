/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

void setup() {
  Serial.begin(115200); // USB
  delay(200);
  Serial.println("DEBUG: start");

  // HardwareSerial 2, RX=16, TX=17
  Serial2.begin(115200, SERIAL_8N2, 16, 17);
  Serial.println("Serial2 started 115200 8N2 on RX=16 TX=17");
}

void loop() {
  while (Serial2.available()) {
    int b = Serial2.read();
    if (b < 0) continue;
    // print ASCII or escape nonprintables as hex
    if (b >= 32 && b <= 126) Serial.write((char)b);
    else {
      Serial.print("[0x");
      if (b < 16) Serial.print('0');
      Serial.print(b, HEX);
      Serial.print("]");
    }
  }
  delay(10);
}
