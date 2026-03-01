#pragma once

#include <InfluxDbClient.h>
#include "common/wifiConnection.h"

#define INFLUXDB_URL "http://192.168.0.220:8428"
#define INFLUXDB_ORG "org"
#define INFLUXDB_BUCKET "bucket"
#define INFLUXDB_TOKEN "token"
#define INFLUXDB_MEASUREMENT "PlatformIO_AirQuality"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensorDataPoints(INFLUXDB_MEASUREMENT);

void setupInfluxdbService()
{
  sensorDataPoints.addTag("device", DEVICE);
  sensorDataPoints.addTag("sensor", strcmp(DEVICE, "ESP8266") == 0 ? "SCD41" : "SCD30");
  sensorDataPoints.addTag("room", strcmp(DEVICE, "ESP8266") == 0 ? "Büro" : "Wohnzimmer");
}

void sendData(int32_t vocIndex, uint16_t co2)
{
  sensorDataPoints.clearFields();
  sensorDataPoints.addField("vocIndex", vocIndex);

  if (co2 > 0)
  {
    sensorDataPoints.addField("co2", co2);
  }

  client.writePoint(sensorDataPoints);
  client.flushBuffer();
}
