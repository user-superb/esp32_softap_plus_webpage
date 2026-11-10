#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "image_data.h"

// Hardware Pin Configuration (KY-023 Joystick)
#define PIN_VRX 34
#define PIN_VRY 35
#define PIN_SW  32

// SoftAP configuration
const char *ssid = "ESP32-SoftAP";
const char *password = "12345678"; // Minimum 8 characters for WPA2-PSK

// Web server instance on port 80 & WebSocket server on port 81
WebServer server(80);
WebSocketsServer webSocket(81);

// Periodic monitoring interval for SoftAP stations (5 seconds)
const unsigned long STATUS_INTERVAL_MS = 5000;
unsigned long lastStatusTime = 0;
uint8_t lastStationCount = 0;

// Joystick sampling and broadcast timings
const unsigned long SAMPLE_INTERVAL_MS = 25;       // ~40Hz sampling
const unsigned long HEARTBEAT_INTERVAL_MS = 200;    // Heartbeat broadcast every 200ms
const int CHANGE_THRESHOLD = 15;                   // ADC threshold to trigger update

unsigned long lastSampleTime = 0;
unsigned long lastBroadcastTime = 0;
int lastX = -1;
int lastY = -1;
bool lastBtn = false;

// Embedded HTML file symbols
extern const uint8_t index_html_start[] asm("_binary_src_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_src_index_html_end");

void handleRoot() {
  server.send_P(200, "text/html", (const char *)index_html_start, index_html_end - index_html_start);
}

void handleImage() {
  server.send_P(200, "image/jpeg", (const char *)image_data, image_data_len);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[WebSocket] Client [%u] connected from %s\n", num, ip.toString().c_str());
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("[WebSocket] Client [%u] disconnected\n", num);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configure Joystick pins
  pinMode(PIN_SW, INPUT_PULLUP);

  Serial.println("\n--- Starting ESP32 SoftAP, Web Server & WebSockets... ---");

  // Configure and start SoftAP
  if (WiFi.softAP(ssid, password)) {
    Serial.println("SoftAP started successfully.");
  } else {
    Serial.println("Failed to start SoftAP!");
  }

  // Output SoftAP network parameters
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.softAPmacAddress());

  // Configure web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/image.jpg", HTTP_GET, handleImage);

  // Start HTTP web server
  server.begin();
  Serial.println("HTTP Web Server started on port 80");

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket Server started on port 81");
  Serial.println("-----------------------------------------------------");
}

void loop() {
  server.handleClient();
  webSocket.loop();

  unsigned long currentMillis = millis();

  // Joystick sampling & broadcast loop
  if (currentMillis - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    lastSampleTime = currentMillis;

    int rawX = analogRead(PIN_VRX);
    int rawY = analogRead(PIN_VRY);
    bool btnPressed = (digitalRead(PIN_SW) == LOW);

    bool valueChanged = (abs(rawX - lastX) > CHANGE_THRESHOLD) ||
                        (abs(rawY - lastY) > CHANGE_THRESHOLD) ||
                        (btnPressed != lastBtn);

    bool heartbeatDue = (currentMillis - lastBroadcastTime >= HEARTBEAT_INTERVAL_MS);

    if (valueChanged || heartbeatDue) {
      lastX = rawX;
      lastY = rawY;
      lastBtn = btnPressed;
      lastBroadcastTime = currentMillis;

      char json[64];
      snprintf(json, sizeof(json), "{\"x\":%d,\"y\":%d,\"btn\":%s}",
               rawX, rawY, btnPressed ? "true" : "false");
      webSocket.broadcastTXT(json);
    }
  }

  // Periodically check SoftAP connected station count
  uint8_t currentStations = WiFi.softAPgetStationNum();
  if (currentStations != lastStationCount) {
    Serial.printf("[SoftAP] Station count changed: %u -> %u\n", lastStationCount, currentStations);
    lastStationCount = currentStations;
  }

  if (currentMillis - lastStatusTime >= STATUS_INTERVAL_MS) {
    lastStatusTime = currentMillis;
    Serial.printf("[SoftAP] Periodic check: %u connected station(s)\n", currentStations);
  }

  delay(2);
}
