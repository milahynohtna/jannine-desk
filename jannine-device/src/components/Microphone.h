#pragma once

#include "infrastructure/drivers/I2SDriver.h"
#include "config/MicrophoneConfig.h"

// ──────────────────────────────────────────────────────
// Microphone — orchestration layer (pakai I2SDriver)
// ──────────────────────────────────────────────────────
class Microphone {
public:
    Microphone(
        I2SDriver& driver,
        const MicrophonePins& pins = MicrophoneConfig::PINS,
        const MicrophoneSettings& settings = MicrophoneConfig::SETTINGS
    );

    // lifecycle
    void init();
    void start();
    void stop();

    // read
    size_t read(uint8_t* buffer, size_t len);

    // metadata
    uint32_t sampleRate() const;
    uint16_t bufferSize() const;

private:
    I2SDriver& _driver;

    MicrophonePins     _pins;
    MicrophoneSettings _settings;

    bool _started = false;
};