#pragma once

#if defined(ESP32)
#include "esp_task_wdt.h"
#elif defined(ESP8266)
#include <Arduino.h>
#endif

void watchdog_init()
{
#if defined(ESP32)

    esp_task_wdt_init(30, true); // Watchdog-Timer wird alle 30 Sekunden ausgelöst
    esp_task_wdt_add(NULL);      // Fügt den aktuellen Task zum Watchdog-Timer hinzu

#elif defined(ESP8266)

    wdt_enable(WDTO_30S); // Watchdog-Timer wird alle 30 Sekunden ausgelöst

#endif
}

// Watchdog-Timer wird zurückgesetzt, um den Systemneustart zu verhindern!
void watchdog_reset()
{
#if defined(ESP32)

    esp_task_wdt_reset();

#elif defined(ESP8266)

    wdt_reset();

#endif
}
