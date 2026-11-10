# ESP32 SoftAP Web Server & Real-Time Joystick Telemetry

Standalone ESP32 firmware running a Wi-Fi Access Point (SoftAP), HTTP web server, and WebSocket telemetry stream. Features real-time KY-023 analog joystick tracking with an interactive dashboard, plus embedded HTML and JPEG assets stored directly in flash memory without requiring SPIFFS/LittleFS partitions.

<img width="728" height="470" alt="image" src="https://github.com/user-attachments/assets/eebc1453-b1aa-4613-9b62-1f6e2b1ec7cb" />

---

## Features

- **Autonomous Access Point**: Dedicated WPA2-PSK Wi-Fi network (`192.168.4.1`).
- **Real-Time Joystick Telemetry**: Low-latency KY-023 joystick sampling (~40Hz) with delta thresholding (`15` ADC units) and automatic heartbeat broadcast.
- **WebSocket Streaming**: High-speed JSON event stream on port 81 via `links2004/WebSockets`.
- **Interactive Web Interface**: Live virtual joystick visualization, crosshairs, button press feedback, and real-time ADC metrics.
- **Embedded Binary Assets**: HTML/CSS UI (`src/index.html`) embedded directly via `board_build.embed_txtfiles`; JPEG image served from PROGMEM (`include/image_data.h`).
- **Station Telemetry**: Serial monitoring of Wi-Fi client connection events and periodic station counts.

---

## Hardware & Wiring (KY-023 Joystick)

| KY-023 Pin | ESP32 GPIO | Mode / Description |
| :--- | :--- | :--- |
| **GND** | `GND` | Common Ground |
| **+5V / VCC** | `3V3` | Power supply (3.3V recommended for ADC accuracy) |
| **VRx** | `GPIO 34` | X-Axis Analog Input (`ADC1_CH6`) |
| **VRy** | `GPIO 35` | Y-Axis Analog Input (`ADC1_CH7`) |
| **SW** | `GPIO 32` | Push Button (`INPUT_PULLUP`, Active LOW) |

---

## Environment & Dependencies

| Parameter | Specification |
| :--- | :--- |
| **Board** | ESP32 Dev Module (`esp32dev`) |
| **Framework** | Arduino (`framework-arduinoespressif32`) |
| **Toolchain** | PlatformIO |
| **Serial Baud** | 115200 baud |
| **Libraries** | `links2004/WebSockets @ ^2.4.1`, `WebServer`, `WiFi` |

---

## Network & Endpoints

| Parameter | Value |
| :--- | :--- |
| **SSID** | `ESP32-SoftAP` |
| **Password** | `12345678` |
| **Default IP** | `192.168.4.1` |
| **Web Dashboard** | `GET http://192.168.4.1/` (Serves embedded `src/index.html`) |
| **Image Asset** | `GET http://192.168.4.1/image.jpg` (Streams raw JPEG binary) |
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

*One-liner to flash and monitor:*
```bash
pio run -t upload && pio device monitor -b 115200
```

### 4. Connect & Access

1. Connect your PC or mobile device to the Wi-Fi network **`ESP32-SoftAP`** using password **`12345678`**.
2. Open a browser and navigate to `http://192.168.4.1`.
3. The dashboard connects automatically to `ws://192.168.4.1:81` to display real-time joystick motion and button state.

---

## Project Structure

```text
├── include/
│   └── image_data.h       # Raw JPEG byte array stored in PROGMEM
├── src/
│   ├── index.html         # Interactive dashboard UI with WebSocket client
│   └── main.cpp           # SoftAP, WebServer, WebSockets, & Joystick sampling
├── platformio.ini         # PlatformIO config, library dependencies, & embed rules
└── .gitignore             # Ignored build artifacts and system files
```

---

## Customization

### Modifying the Dashboard UI
Edit [`src/index.html`](file:///home/superb/Documentos/PlatformIO/Projects/ESP32/src/index.html) directly with standard HTML/CSS/JavaScript. PlatformIO embeds the file at build time, so no filesystem upload (`uploadfs`) is required.

### Replacing the Embedded Image
1. Convert any image to a C byte array:
   ```bash
   xxd -i image.jpg > image_data.h
   ```
2. Replace the contents of [`include/image_data.h`](file:///home/superb/Documentos/PlatformIO/Projects/ESP32/include/image_data.h) with the resulting array and length constants (`image_data` and `image_data_len`).
