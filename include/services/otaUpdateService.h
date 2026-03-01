#pragma once

#include "common/debug.h"

#if defined(ESP32)
#include <HTTPUpdate.h> // https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPUpdate/examples/httpUpdate/httpUpdate.ino
#elif defined(ESP8266)
#include <ESP8266httpUpdate.h> // https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html
#endif

#define SERVER_IP "192.168.0.220"
#define SERVER_PORT 5678
#define UPDATE_ENDPOINT "/esp-update/air-quality"

// For a new version, today's date must be used here!
/////////////////////////////////////////////////////
#define FW_VERSION "2026-03-01"                    //
/////////////////////////////////////////////////////

void setupUpdateServiceAndUpdate()
{
  debugPrintln("[update] Search for update ...");
  debugPrint("[update] Current version: ");
  debugPrintln(FW_VERSION);

  WiFiClient client;

#if defined(ESP32)
  t_httpUpdate_return ret = httpUpdate.update(client, SERVER_IP, SERVER_PORT, UPDATE_ENDPOINT, FW_VERSION);
#elif defined(ESP8266)
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, SERVER_IP, SERVER_PORT, UPDATE_ENDPOINT, FW_VERSION);
#endif

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    debugPrintln("[update] Update failed.");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    debugPrintln("[update] No Update Available.");
    break;
  case HTTP_UPDATE_OK:
    debugPrintln("[update] Update done."); // not be called since we reboot the ESP
    break;
  }
}
