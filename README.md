# Plane Radar

![plane-radar](https://github.com/user-attachments/assets/716d0992-dab8-47ba-8f1a-2aec7f607419)

**3D printed case (STL + assembly):** [MakerWorld](https://makerworld.com/en/models/2872376-esp32-plane-radar-live-ads-b-on-a-round-display#profileId-3207083) · **Firmware:** [Releases](https://github.com/commputethis/ESP32-Plane-Radar/releases)

Firmware for a **1.28″ round GC9A01** display (240×240) on **ESP32-C3 Super Mini** or **Waveshare ESP32-S3 LCD 1.28"**. Shows a circular **ADS-B radar** around your configured location, with **WiFiManager** for first-time setup.

## What it does

1. **Wi‑Fi setup** (if needed) — captive portal on AP **`PlaneRadar-Setup`**
2. **Radar** — live aircraft from [adsb.fi](https://opendata.adsb.fi/) on a sonar-style grid

After Wi‑Fi is saved, the device reconnects automatically; the radar runs in the main loop with periodic ADS-B updates (~5 s).

## Supported Boards

| Board | Display | Notes |
| ------- | --------- | ------- |
| **ESP32-C3 Super Mini** | GC9A01 round 1.28" (external) | USB-C native (CDC), original design |
| **Waveshare ESP32-S3 LCD 1.28"** | GC9A01 1.28" (onboard) | UART serial, integrated display |

Both environments use the same codebase with conditional compilation (`include/config.h`).

## Controls (BOOT pin, active LOW)

| Board | BOOT Pin |
| ------- | ---------- |
| ESP32-C3 Super Mini | GPIO 9 |
| Waveshare ESP32-S3 | GPIO 0 |

| Action | Effect |
| -------- | -------- |
| **Short tap** | Cycle range preset (5 → 10 → 15 → 25 km); saved to flash |
| **Hold 3 s** | Clear Wi‑Fi, location, and units; reboot into setup portal |

During setup you can also hold BOOT at power-on to force a credential reset (same as the long press).

## Wi‑Fi setup portal

**First-time setup** (no saved Wi‑Fi):

1. Connect to **`PlaneRadar-Setup`**
2. Open **`http://plane-radar.local`** (preferred) or **`http://192.168.4.1`** — both are shown on the yellow setup screen; captive portal may open automatically
3. Set home Wi‑Fi, then save

**Reconfigure anytime** (after the device is on your network):

1. Open **`http://plane-radar.local`** or **`http://<device-ip>`** (e.g. from your router or serial log at boot)
2. Change Wi‑Fi, location, units, or runway overlay; save

The same portal runs on the setup AP and on the device's LAN IP while connected to Wi‑Fi. mDNS hostname is `plane-radar` → **plane-radar.local** (`kPortalHostname` in `config.h`).

**Custom fields** (stored in NVS):

| Field | Purpose |
| ------- | --------- |
| **Latitude / Longitude** | Radar center and ADS-B query position (defaults in `config.h` until set) |
| **Display distances in miles** | Ring scale label in **mi** instead of **km** (e.g. `6mi` vs `10km`) |
| **Show airport runways** | Major-airport runway overlay on the radar (off to hide) |

After a reset, the device reboots and shows the setup screen immediately (no "Connecting" loop on stale credentials).

## Radar display

### Grid

- Dark blue background, subdued green rings and crosshairs
- White **N / S / E / W** at the bezel; range label on the **east** spoke (ring 3 = ¾ of outer radius)
- White center dot

Layout and colors: `include/ui/radar_theme.h`.

### Range presets

| Ring 3 label | Outer radius (aircraft scale) |
| ------------ | ------------------------------- |
| 5 km / 3 mi | ~6.7 km |
| 10 km / 6 mi | ~13.3 km (default) |
| 15 km / 9 mi | ~20 km |
| 25 km / 16 mi | ~33.3 km |

Preset and miles/km choice persist across reboot (`planeradar` NVS namespace).

### Runways

- Major airports from OurAirports (`large_airport`); all open runway strips in range (helipads excluded)
- Teal runway lines with one ICAO label per airport (e.g. `KJFK`); toggle in the Wi‑Fi setup portal
- Update the embedded list: `python3 scripts/build_large_airports.py`

### Aircraft

- **Inside the outer ring** — red heading triangle, magenta speed vector (clipped at the ring), callsign / type / altitude tags
- **Outside the ring** (still within ADS-B fetch) — small **red dot on the screen rim** at the correct bearing (direction cue; not distance-accurate past the ring)
- **Tags** — placed toward the **center**: west (left) → tag on the **right** of the symbol; east (right) → tag on the **left**

As range decreases (or aircraft approach), targets move inward; beyond-ring dots become full symbols when they cross the outer ring.

### ADS-B

- Source: `https://opendata.adsb.fi/api/v3/`
- Fetch radius: `ui::radar::fetchRadiusKm()` — scales with the active preset to roughly the screen edge (so rim dots have data)
- Poll interval: `kAdsbFetchIntervalMs` (5 s) in `config.h`
- Ground aircraft hidden by default (`kAdsbShowGroundAircraft`)

## Configuration

Edit **`include/config.h`** for hardware and behavior. The file uses conditional compilation to support both boards automatically:

| Area | Keys / notes |
| ------ | -------------- |
| Portal | `kPortalApName`, `kPortalIp`, `kPortalHostname` / `kPortalHostUrl` (mDNS; needs `-DWM_MDNS` in `platformio.ini`) |
| Wi‑Fi timing | connect attempts, reconnect grace, portal timeout (`0` = no timeout) |
| BOOT | `kBootPin`, `kBootResetHoldMs`, `kBootTapMinMs` |
| Display SPI | pins auto-selected per board, `kDisplayInvert`, `kDisplayRgbOrder`, `kDisplaySpiWriteHz` |
| Backlight | `kDisplayPinBl` (Waveshare only, GPIO 40) |
| Default location | `kDefaultRadarLat`, `kDefaultRadarLon` (until portal overrides) |
| ADS-B | `kAdsbFetchIntervalMs`, `kAdsbShowGroundAircraft` |

Range presets: `include/ui/radar_range.h` (`kRangePresets`).

## Project layout

```txt
include/
  config.h
  hardware/
    lgfx_config.hpp          — conditional SPI/panel config per board
    display.h
    display_font.h
  data/
    large_airports.h
  ui/
    radar_theme.h
    radar_range.h
    radar_display.h
    runway_overlay.h
    status_screens.h
  services/
    wifi_setup.h
    radar_location.h
    adsb_client.h
data/
  ui_font.vlw                — embedded smooth UI font (Noto Sans Bold)
scripts/
  build_large_airports.py
src/
  main.cpp
  data/
    large_airports_data.cpp
  hardware/
  ui/
  services/
```

## Wiring

### ESP32-C3 Super Mini + External GC9A01

| Display | ESP32-C3 |
| --------- | ---------- |
| VCC | 3V3 |
| GND | GND |
| RST | GPIO **0** |
| CS | GPIO **1** |
| DC | GPIO **10** |
| SDA (MOSI) | GPIO **3** |
| SCL (SCLK) | GPIO **4** |
| BOOT (user) | GPIO **9** |

### Waveshare ESP32-S3 LCD 1.28"

The display is integrated on the board. Pins are routed internally:

| Signal | GPIO | Function |
| -------- | ------ | ---------- |
| LCD_CLK | **10** | SPI Clock |
| LCD_DIN (MOSI) | **11** | SPI Data In |
| LCD_CS | **9** | Chip Select |
| LCD_DC | **8** | Data/Command |
| LCD_RST | **12** | Reset |
| LCD_BL | **40** | Backlight (PWM capable) |
| BOOT | **0** | Boot/Reset button |

**Note:** The Waveshare board includes an onboard IMU ( QMI8658C ) on I2C (GPIO 6/7), but this firmware does not use it.

## Build

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- USB-C cable for programming

### For ESP32-C3 Super Mini (original)

```bash
pio run -e supermini -t upload
pio device monitor
```

- PlatformIO env: **`supermini`**
- Serial: **115200** baud (USB-C CDC)
- USB CDC on boot enabled in `platformio.ini`

### For Waveshare ESP32-S3 LCD 1.28"

```bash
pio run -e waveshare-s3 -t upload
pio device monitor
```

- PlatformIO env: **`waveshare-s3`**
- Serial: **115200** baud (UART only)
- Upload via USB-C using standard ESP32-S3 bootloader
- **Important:** The backlight is controlled by GPIO 40 and is automatically initialized by the firmware

### Entering Download Mode (if needed)

If automatic upload fails, manually enter download mode:

1. Hold **BOOT** button (GPIO 0)
2. Press and release **RESET** (or unplug/replug USB)
3. Release **BOOT** button
4. Upload firmware

### Web-flashable release image

Single `.bin` for [esptool-js](https://espressif.github.io/esptool-js/) and similar tools:

```bash
chmod +x scripts/merge-firmware.sh   # once
./scripts/merge-firmware.sh
```

Writes `release/plane-radar-merged.bin` for the default (`supermini`) environment. To build for Waveshare:

```bash
./scripts/merge-firmware.sh waveshare-s3
```

Or via PlatformIO only:

```bash
pio run -e supermini
pio run -t merge -e supermini
```

Put the board in download mode (hold **BOOT**, tap **RESET**), then flash with Chrome/Edge over USB.

### CI and releases (GitHub Actions)

| Workflow | When | Output |
| ---------- | ------ | -------- |
| [Build](.github/workflows/build.yml) | Push / PR to `main` | Artifacts for both `supermini` and `waveshare-s3` (~90 days) |
| [Release](.github/workflows/release.yml) | Git tag `v*` (e.g. `v1.0.0`) | GitHub Release assets (both environments) |

To ship a version users can download:

```bash
git tag v1.0.0
git push origin v1.0.0
```

The release workflow builds firmware in CI for all environments and attaches merged images to the release.

## Troubleshooting

### Blank screen on Waveshare board

If the serial monitor shows the code is running but the display is blank:

1. **Check build environment** — Make sure you used `pio run -e waveshare-s3`, not `supermini`
2. **Backlight** — The backlight (GPIO 40) is automatically controlled by the firmware. If you've modified the code, ensure `pinMode(40, OUTPUT)` and `digitalWrite(40, HIGH)` are called during display initialization.
3. **Pin conflicts** — GPIO 9 is used for LCD_CS on the Waveshare board, not the BOOT button (which is GPIO 0).

### Wi-Fi connection issues

- The device will create `PlaneRadar-Setup` AP if no valid Wi-Fi credentials are stored
- Hold BOOT for 3+ seconds to clear credentials and restart setup
- Check serial monitor (115200 baud) for connection status and IP address

## Dependencies

- [LovyanGFX](https://github.com/lovyan03/LovyanGFX)
- [WiFiManager](https://github.com/tzapu/WiFiManager)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

## License

MIT License — see [LICENSE](LICENSE)
