# ESP32 SoftAP Web Server & Real-Time Joystick Telemetry

Standalone ESP32 firmware running a Wi-Fi Access Point (SoftAP), HTTP web server, and WebSocket telemetry stream. Features real-time HW-504 / KY-023 analog joystick tracking with a responsive, centered dashboard UI embedded directly in flash memory without requiring SPIFFS/LittleFS partitions.

---

## Features

- **Autonomous Access Point**: Dedicated WPA2-PSK Wi-Fi network (`192.168.4.1`).
- **Real-Time Joystick Telemetry**: Low-latency joystick sampling (~40Hz) with delta thresholding (`15` ADC units) and automatic heartbeat broadcast.
- **WebSocket Streaming**: High-speed JSON event stream on port 81 via `links2004/WebSockets`.
- **Minimalist Centered Interface**: Responsive, dark-mode 2D joystick visualizer with crosshairs, animated thumbstick, button press indicator, and live ADC coordinate metrics.
- **Direct Asset Embedding**: Pure HTML/CSS/JS (`src/index.html`) embedded directly into the binary via `board_build.embed_txtfiles`.
- **Station Telemetry**: Serial monitoring of Wi-Fi client connection events and connected station counts.

---

## Hardware & Wiring (HW-504 / KY-023 Joystick)

| Module Pin | ESP32 GPIO | Description / Notes |
| :--- | :--- | :--- |
| **GND** | `GND` | Common Ground |
| **+5V / VCC** | `3V3` | Power supply (**3.3V required**; do not connect to 5V to protect ADC) |
| **VRx** | `GPIO 34` | X-Axis Analog Input (`ADC1_CH6`, range 0–4095) |
| **VRy** | `GPIO 35` | Y-Axis Analog Input (`ADC1_CH7`, range 0–4095) |
| **SW** | `GPIO 32` | Push Button (`INPUT_PULLUP`, Active on LOW) |

> [!IMPORTANT]
> The ESP32's ADC2 peripheral is disabled when Wi-Fi is active. Always use **ADC1 pins** (GPIO 32–39) for analog readings when running SoftAP or Wi-Fi.

---

## Environment & Dependencies

| Parameter | Specification |
| :--- | :--- |
| **Target Board** | ESP32 Dev Module (`esp32dev`) |
| **Framework** | Arduino (`framework-arduinoespressif32`) |
| **Toolchain** | PlatformIO |
| **Monitor Baud** | 115200 baud |
| **Libraries** | `links2004/WebSockets @ ^2.4.1`, `WebServer`, `WiFi` |

---

## Network & Endpoints

| Parameter | Value |
| :--- | :--- |
| **SSID** | `ESP32-SoftAP` |
| **Password** | `12345678` |
| **Default IP** | `192.168.4.1` |
| **Web Dashboard** | `GET http://192.168.4.1/` (Serves embedded `src/index.html`) |
| **Telemetry Stream** | `ws://192.168.4.1:81/` (Broadcasts `{"x":int,"y":int,"btn":bool}`) |

---

## Quick Start

### 1. Build Firmware

```bash
pio run
```

### 2. Flash to ESP32

```bash
pio run -t upload
```

### 3. Open Serial Monitor

```bash
pio device monitor -b 115200
```

### 4. Connect & Access

1. Connect your PC or mobile device to the Wi-Fi network **`ESP32-SoftAP`** using password **`12345678`**.
2. Open a browser and navigate to `http://192.168.4.1`.
3. The dashboard connects automatically to `ws://192.168.4.1:81` to display real-time joystick motion and button state.

---

## Project Structure

```text
├── src/
│   ├── index.html         # Centered interactive joystick UI & WebSocket client
│   └── main.cpp           # SoftAP, WebServer, WebSockets, & Joystick sampling
├── platformio.ini         # PlatformIO config, library dependencies, & embed rules
├── AGENTS.md              # AI Agent roles and ADHD-friendly communication protocol
└── README.md              # Project documentation
```

---

## How to modify the webpage

### Modifying the Dashboard UI
Edit [`src/index.html`](src/index.html) directly with standard HTML/CSS/JavaScript. PlatformIO embeds the file into the binary at build time, so no filesystem upload (`uploadfs`) is needed.

