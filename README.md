# ESP32 SoftAP Web Server & Embedded Image Viewer

Standalone ESP32 firmware running a Wi-Fi Access Point (SoftAP) and HTTP web server. Serves a responsive HTML page and flash-stored JPEG image directly from memory without requiring external storage or filesystem partitioning (SPIFFS/LittleFS).

<img width="728" height="470" alt="image" src="https://github.com/user-attachments/assets/eebc1453-b1aa-4613-9b62-1f6e2b1ec7cb" />

---

## Hardware & Environment

| Parameter | Specification |
| :--- | :--- |
| **Board** | ESP32 Dev Module (`esp32dev`) |
| **Framework** | Arduino (`framework-arduinoespressif32`) |
| **Toolchain** | PlatformIO |
| **Serial Baud** | 115200 baud |

---

## Network & Endpoints

| Parameter | Value |
| :--- | :--- |
| **SSID** | `ESP32-SoftAP` |
| **Password** | `12345678` |
| **Default IP** | `http://192.168.4.1` |
| **Web UI** | `GET /` (Serves embedded `src/index.html`) |
| **Image Asset** | `GET /image.jpg` (Streams raw JPEG binary) |

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

---

## Project Structure

```text
├── include/
│   └── image_data.h       # Raw JPEG byte array stored in PROGMEM
├── src/
│   ├── index.html         # Standalone HTML/CSS UI embedded into binary
│   └── main.cpp           # SoftAP init, web routing, and station telemetry
├── platformio.ini         # PlatformIO build configuration & text embed rule
└── .gitignore             # Ignored build artifacts and system files
```

---

## Customization

### Modifying the Web Page
Edit [`src/index.html`](file:///home/superb/Documentos/PlatformIO/Projects/ESP32/src/index.html) directly with standard HTML/CSS. Rebuilding with `pio run` embeds the updated file automatically into the flash binary.

### Replacing the Image
1. Convert any image to a C byte array:
   ```bash
   xxd -i image.jpg > image_data.h
   ```
2. Update [`include/image_data.h`](file:///home/superb/Documentos/PlatformIO/Projects/ESP32/include/image_data.h) with the resulting array and length constants (`image_data` and `image_data_len`).
