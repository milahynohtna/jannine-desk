
#pragma once

#include "../src/config/AudioConfig.h"


class IAudioDriver {
public:
    virtual ~IAudioDriver() = default;

    virtual void setup(const AudioPins& pins, const AudioSettings& settings) = 0;
    virtual bool write(const uint8_t* data, size_t length, TickType_t timeout_ms = 2000) = 0;

    virtual void   flush()          = 0;
    virtual void   drain()          = 0;
    virtual size_t queueAvailable() const = 0;
    virtual bool   isBusy()         const = 0;
};
