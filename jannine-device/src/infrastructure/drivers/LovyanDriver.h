#pragma once

#include "../../../include/display/IDisplayDriver.h"
#include "config/DisplayConfig.h"
#include "images/ImageDescriptor.h"
#include <LovyanGFX.hpp>

class LovyanDriver : public IDisplayDriver {
private:
    class LGFX : public lgfx::LGFX_Device {
        lgfx::Panel_GC9A01 _panel;
        lgfx::Bus_SPI _bus;

    public:
        LGFX(const DisplayPins& pins, const DisplaySettings& settings);

    private:
        void configureBus(const DisplayPins& pins, const DisplaySettings& settings);
        void configurePanel(const DisplayPins& pins, const DisplaySettings& settings);
    };

    LGFX _tft;
    bool _initialized = false;
    uint8_t _rotation;

public:
    explicit LovyanDriver(
        const DisplayPins& pins = DisplayConfig::PINS,
        const DisplaySettings& settings = DisplayConfig::SETTINGS
    );

    bool init() override;
    void setRotation(int rotation) override;
    void clear(uint16_t color) override;


    void showImage(const ImageDescriptor& img) override;
    void showAnimation(const Animation& animation) override;


    void drawText(const char* text, int x, int y, uint16_t color) override;
};
