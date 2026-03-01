#pragma once

#if defined(ESP32)
  #define DEVICE "ESP32"
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
#elif defined(ESP8266)
  #define DEVICE "ESP8266"
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
#else
  #error "Unsupported platform"
#endif

#include "debug.h"
#include "wifiCredentials.h"

namespace WiFiConfig {
  constexpr uint16_t RETRY_DELAY_MS = 500;
}

void waitForWiFiConnection() {
  while (wifiMulti.run() != WL_CONNECTED) {
    debugPrint(".");
    delay(WiFiConfig::RETRY_DELAY_MS);
  }

  debugPrintf("\n[WiFi] Connected to %s (IP: %s)\n",
    WiFi.SSID().c_str(),
    WiFi.localIP().toString().c_str()
  );
}

void setupWiFi(bool connectImmediately = true) {
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  debugPrintf("[WiFi] Device: %s – connecting to \"%s\"...\n",
    DEVICE,
    WIFI_SSID
  );

  if (connectImmediately) {
    waitForWiFiConnection();
  }
}

void disconnectWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  debugPrintln("[WiFi] Disconnected.");
}

/** @return Current signal strength in dBm (0 = not connected) */
int8_t getSignalStrength() {
  return WiFi.isConnected() ? WiFi.RSSI() : 0;
}

/** @return true if currently connected */
bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
