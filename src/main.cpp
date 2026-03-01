#include <Arduino.h>
#include <OneButton.h>
#include <Wire.h>

#include "Adafruit_SGP40.h"
#include "Adafruit_SHT31.h"
#include "common/runtimeHelper.h"
#include "common/watchdogHelper.h"
#include "common/wifiConnection.h"
#include "display/LcdDisplay.h"
#include "services/co2SensorReader.h"
#include "services/influxdbService.h"
#include "services/otaUpdateService.h"

Adafruit_SGP40 sgp40;
Adafruit_SHT31 sht31;
OneButton button(14);

float lastValidTemp = 25.0f;
float lastValidHumidity = 50.0f;

void IRAM_ATTR handleInterrupt()
{
  button.tick();
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  if (!sgp40.begin())
  {
    debugPrintln("SGP40 sensor not found!");
  }

  if (!sht31.begin(0x44))
  { // Set to 0x45 for alternate i2c addr
    debugPrintln("SHT31 sensor not found!");
  }

  setupCo2SensorReader();
  setupLcdAndIcons("Any updates?");
  setupWiFi();
  setupUpdateServiceAndUpdate();
  setupInfluxdbService();
  disconnectWiFi();

  LcdDisplay::showWarmupPage();

  attachInterrupt(digitalPinToInterrupt(14), handleInterrupt, CHANGE);

  button.setClickMs(100);
  button.setPressMs(2500);
  button.attachClick(LcdDisplay::onBacklightButtonClick);
  button.attachLongPressStart([]()
                              { lcd.setColorWhite(); lcd.clear(); ESP.restart(); });

  watchdog_init();
}

void loop()
{
  button.tick();

  if (checkSensorDataIntervalToContinue())
    return;

  watchdog_reset();

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t) && !isnan(h))
  {
    lastValidTemp = t;
    lastValidHumidity = h;
  }
  else
  {
    debugPrintln("SHT31 read failed, using last valid T/H.");
    t = lastValidTemp;
    h = lastValidHumidity;
  }

  int32_t vocIndex = sgp40.measureVocIndex(t, h);

  if (vocIndex == 0)
    return;

  debugPrint("Temp *C = ");
  debugPrint(t);
  debugPrint("\t\t");
  debugPrint("Hum. % = ");
  debugPrintln(h);
  debugPrint("Voc Index: ");
  debugPrintln(vocIndex);

  uint16_t co2 = readNewCo2ValueOrGetCached();

  debugPrint("CO2: ");
  debugPrint(co2);
  debugPrintln(" ppm");
  debugPrintln("");

  LcdDisplay::showResultPage(vocIndex, co2);

  if (checkTransmitDataCounterToContinue())
    return;

  waitForWiFiConnection();
  sendData(vocIndex, co2);
  disconnectWiFi();
}
