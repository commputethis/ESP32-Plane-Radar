#include "hardware/display.h"

#include "hardware/display_font.h"

LGFX tft;

void displayInit() {
  tft.init();
  tft.setRotation(0);
  tft.setBrightness(255);
  tft.setTextWrap(false);
  displayFontInit();

  #ifdef HAS_BACKLIGHT_PIN
    // Initialize backlight for Waveshare board
    pinMode(config::kDisplayPinBl, OUTPUT);
    digitalWrite(config::kDisplayPinBl, HIGH);  // Turn on backlight
    #endif
}
