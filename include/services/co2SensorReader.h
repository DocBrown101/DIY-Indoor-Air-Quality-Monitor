#pragma once

#include <Arduino.h>
#include "SparkFun_SCD30_Arduino_Library.h"
#include "SparkFun_SCD4x_Arduino_Library.h"
#include "common/debug.h"

SCD30 scd30;
SCD4x scd41;

bool isSCD30Available = false;
bool isSCD41Available = false;
uint16_t previousCo2 = 0;

void setupCo2SensorReader()
{
  isSCD30Available = scd30.begin();
  isSCD41Available = scd41.begin();

  if (!isSCD30Available)
  {
    debugPrintln("SCD30 sensor not found!");
  }
  if (!isSCD41Available)
  {
    debugPrintln("SCD41 sensor not found!");
  }
}

uint16_t readNewCo2ValueOrGetCached()
{
  uint16_t co2 = 0;

  if (isSCD30Available || isSCD41Available)
  {
    uint16_t currentCo2 = 0;

    if (isSCD30Available && scd30.dataAvailable())
    {
      currentCo2 = scd30.getCO2();
    }
    else if (isSCD41Available && scd41.readMeasurement())
    {
      currentCo2 = scd41.getCO2();
    }

    if (currentCo2 == 0 && previousCo2 > 0)
    {
      co2 = previousCo2;
    }
    else if (currentCo2 > 0)
    {
      co2 = currentCo2;
      previousCo2 = currentCo2;
    }
  }

  return co2;
}
