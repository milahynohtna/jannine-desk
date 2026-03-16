#pragma once

#include <Arduino.h>
#include "driver/i2s.h"
#include "../../config/AudioConfig.h"

class I2SDriver
{
public:
    I2SDriver() = default;

    void begin(
        const AudioPins& pins     = AudioConfig::PINS,
        const AudioSettings& settings = AudioConfig::SETTINGS
    );

    void write(const uint8_t* data, size_t len);

    void stop();

private:
    i2s_port_t _port = I2S_NUM_0;
};