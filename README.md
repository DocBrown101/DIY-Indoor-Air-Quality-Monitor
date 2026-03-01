# Indoor Air Quality Monitor

Firmware for an ESP32/ESP8266-based air quality monitor.  
Reads VOC index, CO2, temperature, and humidity, displays results on an RGB LCD, and pushes data to InfluxDB or VictoriaMetrics

## Hardware

| Component               | Role                            |
| ----------------------- | ------------------------------- |
| ESP32 or ESP8266        | Microcontroller                 |
| Adafruit SGP40          | VOC index sensor (I2C)          |
| SparkFun SCD30 or SCD41 | CO2 sensor (I2C, auto-detected) |
| DFRobot RGB LCD 1602    | 16×2 display with RGB backlight |

## Features

- **Air quality thresholds** with visual backlight feedback:
  - CO2 warning ≥ 1600 ppm → dim white
  - CO2 alarm ≥ 2000 ppm → pulsing red
  - VOC alarm ≥ 400 index → magenta
- **Hysteresis** on CO2 alarms — requires 60 s sustained reading before triggering or resetting
- **InfluxDB integration** — pushes a data point roughly every 60 s
- **OTA firmware updates** via HTTP on startup
- **Hardware watchdog** — auto-restarts on hang

- Button: short press toggles backlight, long press (2.5 s) factory-resets and restarts to check for updates

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (CLI or VS Code extension)

### Configuration

Copy the credentials example and fill in your values:

```bash
cp include/common/wifiCredentials.example.h include/common/wifiCredentials.h
```

Edit `wifiCredentials.h`:

```cpp
#define WIFI_SSID     "your-network"
#define WIFI_PASSWORD "your-password"
```

Edit `include/services/influxdbService.h` with your InfluxDB URL, org, bucket, and token.

### Build & Flash

```bash
# ESP32 (default)
pio run -e ESP32-dev -t upload

# ESP8266
pio run -e ESP8266-nodemcuv2 -t upload
```

### Serial Monitor

```bash
pio device monitor --baud 115200
```

## License

MIT
