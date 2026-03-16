#include "LovyanDriver.h"

/* ===== LGFX ===== */

LovyanDriver::LGFX::LGFX(
    const DisplayPins& pins,
    const DisplaySettings& settings
) {
    configureBus(pins, settings);
    configurePanel(pins, settings);
}

void LovyanDriver::LGFX::configureBus(
    const DisplayPins& pins,
    const DisplaySettings& settings
) {
    auto cfg = _bus.config();
    cfg.spi_host = SPI2_HOST;
    cfg.spi_mode = 0;
    cfg.freq_write = settings.spi_freq_write;
    cfg.freq_read  = settings.spi_freq_read;
    cfg.spi_3wire  = true;
    cfg.use_lock   = true;
    cfg.dma_channel = SPI_DMA_CH_AUTO;
    cfg.pin_sclk = pins.sclk;
    cfg.pin_mosi = pins.mosi;
    cfg.pin_miso = -1;
    cfg.pin_dc   = pins.dc;
    _bus.config(cfg);
    _panel.setBus(&_bus);
}

void LovyanDriver::LGFX::configurePanel(
    const DisplayPins& pins,
    const DisplaySettings& settings
) {
    auto cfg = _panel.config();
    cfg.pin_cs = pins.cs;
    cfg.pin_rst = pins.rst;
    cfg.panel_width  = settings.panel_width;
    cfg.panel_height = settings.panel_height;
    cfg.invert = settings.invert;
    cfg.rgb_order = settings.rgb_order;
    _panel.config(cfg);
    setPanel(&_panel);
}

/* ===== LovyanDriver ===== */

LovyanDriver::LovyanDriver(
    const DisplayPins& pins,
    const DisplaySettings& settings
) : _tft(pins, settings),
    _rotation(settings.rotation) {}

bool LovyanDriver::init() {
    if (_initialized) return true;
    _tft.init();
    _tft.setSwapBytes(true);
    _tft.setRotation(_rotation);
    _initialized = true;
    return true;
}

void LovyanDriver::setRotation(int rotation) {
    _tft.setRotation(rotation);
}

void LovyanDriver::clear(uint16_t color) {
    _tft.fillScreen(color);
}

void LovyanDriver::showImage(const ImageDescriptor& img) {
    _tft.pushImage(
        img.x,
        img.y,
        img.width,
        img.height,
        img.data
    );
}

void LovyanDriver::showAnimation(const Animation& animation) {
    showImage(animation.currentFrame());
}



void LovyanDriver::drawText(
    const char* text,
    int x,
    int y,
    uint16_t color
) {
    _tft.setTextColor(color);
    _tft.setCursor(x, y);
    _tft.print(text);
}
