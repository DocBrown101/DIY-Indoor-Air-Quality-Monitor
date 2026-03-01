#pragma once

#include <Arduino.h>

const unsigned long SensorDataInterval = 3500UL; // 3,5 Sekunden

unsigned int TransmitDataCounter = 0;
unsigned long previousSensorMillis = 0UL;

bool checkSensorDataIntervalToContinue()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousSensorMillis > SensorDataInterval)
  {
    previousSensorMillis = currentMillis;
    return false; // continue
  }
  return true; // skip here
}

bool checkTransmitDataCounterToContinue()
{
  if (++TransmitDataCounter >= 17)
  {
    TransmitDataCounter = 0;
    return false; // continue
  }
  return true; // skip here
}
