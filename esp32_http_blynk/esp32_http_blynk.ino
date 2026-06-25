/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/************************************************************
 * ESP32 HTTP Server → Blynk Phone App
 * 
 * Receives RFID tag data from Python script via HTTP
 * Forwards to Blynk phone app via WiFi
 * 
 * SETUP:
 * 1. Upload this sketch to ESP32
 * 2. Open Serial Monitor (115200 baud)
 * 3. Note the IP address displayed
 * 4. Run Python script with ESP32's IP address
 ************************************************************/

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID   "TMPL3ajs_Zavb"
#define BLYNK_TEMPLATE_NAME "ESP32RFID"
#define BLYNK_AUTH_TOKEN    "Pik0pCvfvUu8X4-wQTGh35Rutgzo1t7z"

#include <WiFi.h>
#include <WebServer.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
const char* ssid = "CMF";
const char* password = "123456789";

// LED Pin
const int LED_PIN = 2;

// Web server on port 80
WebServer server(80);

// Tag counter
int tagCount = 0;

// Handle incoming tag data
void handleTag() {
  if (server.hasArg("epc")) {
    String tagEPC = server.arg("epc");
    
    tagCount++;
    
    // Blink LED
    digitalWrite(LED_PIN, HIGH);
    
    // Print to Serial
    Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.print("📡 RFID TAG #");
    Serial.println(tagCount);
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.print("EPC: ");
    Serial.println(tagEPC);
    Serial.print("Time: ");
    Serial.print(millis() / 1000);
    Serial.println("s");
    
    // Send to Blynk
    if (Blynk.connected()) {
      String message = "Tag #" + String(tagCount) + ": " + tagEPC;
      Blynk.virtualWrite(V0, message);
      Serial.println("✅ Sent to Blynk");
    } else {
      Serial.println("⚠️  Blynk not connected!");
    }
    Serial.println();
    
    // LED off
    delay(200);
    digitalWrite(LED_PIN, LOW);
    
    // Send success response
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing EPC parameter");
  }
}

// Handle root page
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP32 RFID → Blynk Server</h1>";
  html += "<p>Status: <b>Running</b></p>";
  html += "<p>Tags received: <b>" + String(tagCount) + "</b></p>";
  html += "<p>Blynk: <b>" + String(Blynk.connected() ? "Connected" : "Disconnected") + "</b></p>";
  html += "<hr>";
  html += "<p>Send tag data: <code>http://" + WiFi.localIP().toString() + "/tag?epc=TAG_ID</code></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n");
  Serial.println("╔═════════════════════════════════════════════╗");
  Serial.println("║  ESP32 HTTP → BLYNK RECEIVER v1.0          ║");
  Serial.println("╚═════════════════════════════════════════════╝\n");
  
  // Connect to WiFi
  Serial.print("📶 Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi Connected!");
    Serial.print("📍 IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("📶 Signal: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("❌ WiFi Connection FAILED!");
    Serial.println("⚠️  Check SSID/password and restart ESP32");
    while(1) {
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }
  }
  
  Serial.println();
  
  // Connect to Blynk
  Serial.println("📱 Connecting to Blynk...");
  Blynk.config(BLYNK_AUTH_TOKEN);
  
  if (Blynk.connect()) {
    Serial.println("✅ Blynk Connected!");
    Blynk.virtualWrite(V0, "ESP32 HTTP Server Ready");
  } else {
    Serial.println("⚠️  Blynk connection failed (will retry)");
    Serial.println("   Server will still work, but no phone updates");
  }
  
  Serial.println();
  
  // Setup HTTP server
  server.on("/", handleRoot);
  server.on("/tag", handleTag);
  server.begin();
  
  Serial.println("🌐 HTTP Server started!");
  Serial.println("════════════════════════════════════════════");
  Serial.println();
  Serial.println("📋 READY TO RECEIVE TAGS!");
  Serial.println();
  Serial.println("Python script command:");
  Serial.print("  python3 rfid_to_esp32.py ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("Test command:");
  Serial.print("  curl 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("/tag?epc=TEST1234'");
  Serial.println();
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
  server.handleClient();
  
  // Reconnect to Blynk if disconnected
  if (!Blynk.connected()) {
    static unsigned long lastReconnect = 0;
    if (millis() - lastReconnect > 30000) {  // Try every 30 seconds
      Serial.println("🔄 Reconnecting to Blynk...");
      if (Blynk.connect()) {
        Serial.println("✅ Blynk reconnected!");
      }
      lastReconnect = millis();
    }
  }
}
