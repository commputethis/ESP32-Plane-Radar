#define LGFX_USE_V1

#if defined(LGFX_WAVESHARE_S3)
  // Waveshare ESP32-S3 LCD 1.28" (GC9A01, non-touch)
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
        cfg.pin_mosi = 35;    // DIN on Waveshare
        cfg.pin_miso = 37;    // Not used but required
        cfg.pin_sclk = 36;    // CLK on Waveshare
        cfg.pin_cs = 8;       // CS on Waveshare
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();
        cfg.pin_cs = 8;
        cfg.pin_rst = 4;      // RST on Waveshare
        cfg.pin_busy = -1;
        cfg.offset_x = 0;
        cfg.offset_y = 0;
        cfg.panel_height = 240;
        cfg.panel_width = 240;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits = 1;
        cfg.readable = true;
        cfg.invert = true;
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
