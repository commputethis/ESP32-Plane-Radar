#pragma once
#include <cstdint>
#include <driver/gpio.h>

namespace config {

// --- Board detection ---
#if defined(BOARD_WAVESHARE_S3)
    #define IS_WAVESHARE_S3
#elif defined(BOARD_SUPERMINI)
    #define IS_SUPERMINI
#else
    // Default to Super Mini for backward compatibility
    #define IS_SUPERMINI
#endif

// --- Wi-Fi portal ---
constexpr char kPortalApName[] = "PlaneRadar-Setup";
constexpr char kPortalIp[] = "192.168.4.1";
constexpr char kPortalHostname[] = "plane-radar";
constexpr char kPortalHostUrl[] = "plane-radar.local";

constexpr unsigned long kWifiConnectAttemptMs = 15000;
constexpr uint8_t kWifiConnectAttempts = 3;
constexpr unsigned long kWifiPortalTimeoutSec = 0;
constexpr unsigned long kWifiConnectingFrameMs = 50;
constexpr unsigned long kWifiDownGraceMs = 4000;
constexpr unsigned long kWifiReconnectIntervalMs = 15000;

// --- BOOT button (active LOW) ---
#ifdef IS_WAVESHARE_S3
    constexpr gpio_num_t kBootPin = GPIO_NUM_0;  // ESP32-S3 BOOT button
#else
    constexpr gpio_num_t kBootPin = GPIO_NUM_9;  // ESP32-C3 Super Mini
#endif

constexpr unsigned long kBootResetHoldMs = 3000UL;
constexpr unsigned long kBootTapMinMs = 40UL;

// --- Display: GC9A01 1.28" round 240×240 (SPI) ---
#ifdef IS_WAVESHARE_S3
    // Waveshare ESP32-S3 LCD 1.28" pinout
    constexpr gpio_num_t kDisplayPinRst = GPIO_NUM_12;
    constexpr gpio_num_t kDisplayPinCs  = GPIO_NUM_9;
    constexpr gpio_num_t kDisplayPinDc  = GPIO_NUM_8;
    constexpr gpio_num_t kDisplayPinMosi = GPIO_NUM_11;  // LCD DIN
    constexpr gpio_num_t kDisplayPinSclk = GPIO_NUM_10;  // LCD CLK
    constexpr gpio_num_t kDisplayPinBl  = GPIO_NUM_40;   // Backlight
    #define HAS_BACKLIGHT_PIN
#else
    // ESP32-C3 Super Mini pinout
    constexpr gpio_num_t kDisplayPinRst = GPIO_NUM_0;
    constexpr gpio_num_t kDisplayPinCs  = GPIO_NUM_1;
    constexpr gpio_num_t kDisplayPinDc  = GPIO_NUM_10;
    constexpr gpio_num_t kDisplayPinMosi = GPIO_NUM_3;   // display SDA
    constexpr gpio_num_t kDisplayPinSclk = GPIO_NUM_4;   // display SCL
    // No dedicated backlight pin on Super Mini setup
#endif

constexpr int kDisplayWidth = 240;
constexpr int kDisplayHeight = 240;
constexpr uint32_t kDisplaySpiWriteHz = 40000000;
constexpr bool kDisplayInvert = true;
constexpr bool kDisplayRgbOrder = true;

// --- Radar center defaults ---
constexpr double kDefaultRadarLat = 52.3676;
constexpr double kDefaultRadarLon = 4.9041;

constexpr unsigned long kAdsbFetchIntervalMs = 3000;
constexpr float kAdsbFetchRadiusScale = 1.0f;
constexpr bool kAdsbShowGroundAircraft = false;

// --- UI colors (RGB565) ---
constexpr uint16_t kColorBlack = 0x0000;
constexpr uint16_t kColorYellow = 0xFFE0;
constexpr uint16_t kTextOnYellow = kColorBlack;
constexpr uint16_t kTextOnBlack = 0xFFFF;

} // namespace config