#define LGFX_USE_V1

#if defined(LGFX_WAVESHARE_S3)
  // Waveshare ESP32-S3 LCD 1.28" (GC9A01, non-touch)
  // Pinout: https://github.com/Waveshare/ESP32-S3-LCD-1.28
  #include <lgfx/v1/panel/Panel_GC9A01.hpp>
  #include <lgfx/v1/bus/Bus_SPI.hpp>

  class LGFX : public lgfx::LGFX_Device
  {
    lgfx::Bus_SPI _bus_instance;
    lgfx::Panel_GC9A01 _panel_instance;

  public:
    LGFX(void)
    {
      {
        auto cfg = _bus_instance.config();
        cfg.spi_host = SPI2_HOST;
        cfg.spi_mode = 0;
        cfg.freq_write = 80000000;
        cfg.freq_read = 16000000;
        cfg.pin_mosi = 11;    // LCD_DIN (MOSI)
        cfg.pin_miso = -1;    // Not used (display is write-only)
        cfg.pin_sclk = 10;    // LCD_CLK (SCLK)
        cfg.pin_cs = 9;       // LCD_CS (Chip Select)
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();
        cfg.pin_cs = 9;       // LCD_CS
        cfg.pin_rst = 12;     // LCD_RST (Reset)
        cfg.pin_dc = 8;       // LCD_DC (Data/Command)
        cfg.pin_busy = -1;    // Not used
        cfg.offset_x = 0;
        cfg.offset_y = 0;
        cfg.panel_height = 240;
        cfg.panel_width = 240;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits = 1;
        cfg.readable = false;  // Display doesn't support readback
        cfg.invert = false;
        cfg.rgb_order = false;
        cfg.memory_width = 240;
        cfg.memory_height = 240;
        _panel_instance.config(cfg);
      }

      setPanel(&_panel_instance);
    }
  };

#else
  // ESP32-C3 Super Mini + 1.28" round GC9A01 (default)
  #include <lgfx/v1/panel/Panel_GC9A01.hpp>
  #include <lgfx/v1/bus/Bus_SPI.hpp>

  class LGFX : public lgfx::LGFX_Device
  {
    lgfx::Bus_SPI _bus_instance;
    lgfx::Panel_GC9A01 _panel_instance;

  public:
    LGFX(void)
    {
      {
        auto cfg = _bus_instance.config();
        cfg.spi_host = SPI2_HOST;
        cfg.spi_mode = 3;
        cfg.freq_write = 80000000;
        cfg.freq_read = 16000000;
        cfg.pin_mosi = 2;
        cfg.pin_miso = 1;
        cfg.pin_sclk = 3;
        cfg.pin_cs = 4;
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();
        cfg.pin_cs = 4;
        cfg.pin_rst = 5;
        cfg.pin_dc = 10;
        cfg.pin_busy = -1;
        cfg.offset_x = 0;
        cfg.offset_y = 0;
        cfg.panel_height = 240;
        cfg.panel_width = 240;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits = 1;
        cfg.readable = true;
        cfg.invert = false;
        cfg.rgb_order = true;  // BGR panel: swap R/B
        cfg.memory_width = 240;
        cfg.memory_height = 240;
        _panel_instance.config(cfg);
      }

      setPanel(&_panel_instance);
    }
  };

#endif
