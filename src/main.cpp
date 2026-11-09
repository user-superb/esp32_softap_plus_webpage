#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "image_data.h"

// SoftAP configuration
const char *ssid = "ESP32-SoftAP";
const char *password = "12345678"; // Minimum 8 characters for WPA2-PSK

// Web server instance on port 80
WebServer server(80);

// Periodic monitoring interval (5 seconds)
const unsigned long STATUS_INTERVAL_MS = 5000;
unsigned long lastStatusTime = 0;
uint8_t lastStationCount = 0;

// Embedded HTML file symbols
extern const uint8_t index_html_start[] asm("_binary_src_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_src_index_html_end");

void handleRoot() {
  server.send_P(200, "text/html", (const char *)index_html_start, index_html_end - index_html_start);
}

void handleImage() {
  server.send_P(200, "image/jpeg", (const char *)image_data, image_data_len);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n--- Starting ESP32 SoftAP & Web Server ---");

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

  // Start web server
  server.begin();
  Serial.println("HTTP Web Server started on port 80");
  Serial.println("------------------------------------------");
}

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();
  uint8_t currentStations = WiFi.softAPgetStationNum();

  // Log immediately if connected station count changes
  if (currentStations != lastStationCount) {
    Serial.printf("[SoftAP] Station count changed: %u -> %u\n", lastStationCount, currentStations);
    lastStationCount = currentStations;
  }

  // Periodically log station count every 5 seconds
  if (currentMillis - lastStatusTime >= STATUS_INTERVAL_MS) {
    lastStatusTime = currentMillis;
    Serial.printf("[SoftAP] Periodic check: %u connected station(s)\n", currentStations);
  }

  delay(2);
}
