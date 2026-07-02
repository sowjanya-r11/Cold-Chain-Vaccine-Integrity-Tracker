# ColdTrace — Cold-Chain Vaccine Integrity Tracker

An IoT-based real-time monitoring system that ensures the integrity of temperature-sensitive vaccines during storage and transportation.

## Overview

Vaccines require strict cold-chain management to remain effective — roughly 25% of vaccines are estimated to arrive at their destination degraded due to temperature excursions during transit. ColdTrace addresses this by continuously monitoring temperature, humidity, light exposure, and physical disturbances (shock/tilt), then surfacing that data through a live dashboard with instant alerting.

The system combines embedded sensing (ESP32 + sensor suite), wireless communication, GPS tracking, and a web-based dashboard into a single end-to-end cold-chain monitoring solution suitable for hospitals, pharmaceutical warehouses, ambulances, and vaccine distribution centers.

## Key Features

- **Multi-parameter monitoring** — temperature, humidity, light exposure, and motion (shock/tilt) in one system, rather than temperature alone.
- **Multi-sensor fusion** — combines environmental data (temp/humidity), physical risk data (shock/tilt/mishandling), and access data (light/door sensors) into a unified view.
- **Real-time GPS tracking** — live location of the vaccine container for route monitoring, delay identification, and route-deviation alerts.
- **Integrity Scoring** — a real-time composite score (0–100) that distills raw sensor data into a simple, actionable metric for healthcare and logistics staff.
- **Immediate alerting** — visual (RGB LED) and audible (buzzer) alerts on threshold breaches, plus SMS dispatch via GSM when internet connectivity is unavailable.
- **3D box simulator** — an interactive Wokwi-based simulation for testing box states (open/closed, tilt, shake, route deviation) without physical hardware.
- **Audit logging** — a timestamped history of all sensor events and system actions for compliance and traceability.

## System Architecture

```
Sensors (DHT22, DS18B20/MAX30205, LDR, MPU6050, Hall Effect, NEO-6M GPS)
        │
        ▼
   ESP32 Microcontroller  ──►  Buzzer / RGB LED (local alerts)
        │                 ──►  SIM800L GSM (SMS fallback alerts)
        ▼
  WebSocket / Node.js + Express Backend
        │
        ▼
     MongoDB (historical data storage)
        │
        ▼
  Web Dashboard (Live monitoring, Alerts, Audit Log, 3D Box Sim)
```

## Hardware Components

**Microcontroller**
- ESP32 Development Board

**Sensors**

| Sensor | Function | Wiring |
|---|---|---|
| MAX30205 | Precision temperature monitoring | VCC–3.3V, SDA–GPIO21, SCL–GPIO22 |
| DHT22 | Humidity monitoring | VCC–3.3V, DATA–GPIO4 |
| DS18B20 | Temperature measurement | OneWire bus |
| LDR | Detects unauthorized light exposure / box opening | VCC–3.3V, OUT–GPIO34 (ADC) |
| MPU6050 | Accelerometer + gyroscope for shock/tilt detection | VCC–3.3V, SDA–GPIO21, SCL–GPIO22 |
| Hall Effect Sensor (A3144) | Door/lid open-close detection via magnetic field | VCC–3.3V, OUT–GPIO35 |
| NEO-6M GPS | Real-time location tracking | VCC–5V, TX–GPIO16, RX–GPIO17 |

**Actuators / Modules**

| Module | Purpose |
|---|---|
| Buzzer | Audible alert on abnormal conditions (high temp, drop, tampering) |
| RGB LED | Visual system status (Normal / Warning / Error) |
| SIM800L (GSM) | SMS alerts / data transmission when offline |

## Alert Conditions

The system raises alerts for:
- **Temperature breach** — outside the safe range (2–8 °C)
- **Humidity out of range** — outside the safe range (40–75%)
- **Box/lid opened** — unauthorized access via Hall sensor / LDR
- **Route deviation** — GPS position moves outside the approved geofence corridor
- **Impact / shake** — shock or tilt beyond the configured threshold, via MPU6050

Each alert must be individually acknowledged by an operator and is logged with a timestamp and source sensor in the Audit Log.

## Dashboard Modules

- **Live Dashboard** — real-time temperature, humidity, shock level, box status, GPS speed, and overall Integrity Score
- **Alerts** — categorized active alerts (Chain Breach, Route Deviation, Unauthorized Access) with acknowledgment workflow and SMS dispatch log
- **Audit Log** — full chronological event history across all sensors and system actions
- **3D Box Simulator** — interactive simulation for testing box open/close, tilt, shake, and route-deviation scenarios
- **Settings** — system configuration

## Firmware

Two firmware variants are included:

1. **Wokwi Simulation Sketch** — full sensor suite (DHT22, DS18B20, LDR, MPU6050) with combined threshold-based alerting and RGB LED / buzzer feedback, using the Adafruit MPU6050 and DallasTemperature libraries.
2. **Arduino IDE Sketch (DHT22-focused)** — a standalone DHT22 reader with input validation, threshold-based LED alerting, and JSON-formatted serial output for backend ingestion.

### Firmware Dependencies

- `Wire.h`
- `Adafruit_MPU6050`
- `Adafruit_Sensor`
- `DHT` (Adafruit DHT sensor library)
- `OneWire`
- `DallasTemperature`

## Tech Stack

- **Microcontroller:** ESP32
- **Backend:** Node.js, Express
- **Database:** MongoDB
- **Real-time communication:** Socket.IO / WebSocket
- **Frontend:** Web-based live dashboard with Chart.js-style visualizations
- **Simulation:** Wokwi (3D circuit simulation)

## Known Limitations / Challenges

- Wokwi serial port access has limitations on Windows during hardware-in-the-loop testing.
- Integrating the pre-built frontend without a full rebuild required careful API/data-contract alignment between firmware, backend, and UI.

## Demo

Wokwi simulation: https://wokwi.com/projects/457575405557011457

## Deployment Use Cases

- Hospitals and pharmacies
- Pharmaceutical warehouses
- Ambulances and transport vehicles
- Vaccine distribution centers
