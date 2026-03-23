#pragma once

#include "driver/i2s.h"
#include <cstddef>
#include <cstdint>

// forward declaration
struct AudioPins;
struct AudioSettings;

struct MicrophonePins;
struct MicrophoneSettings;

// ──────────────────────────────────────────────────────
// I2S Mode State
// ──────────────────────────────────────────────────────
enum class I2SMode {
    NONE,
    TX,
    RX
};

// ──────────────────────────────────────────────────────
// I2SDriver
// ──────────────────────────────────────────────────────
class I2SDriver {
public:
    explicit I2SDriver(i2s_port_t port = I2S_NUM_0);

    // TX (Speaker)
    void begin(
        const AudioPins& pins,
        const AudioSettings& settings
    );

    void write(const uint8_t* data, size_t len);

    // RX (Microphone)
    void beginMic(
        const MicrophonePins& pins,
        const MicrophoneSettings& settings
    );

    size_t read(uint8_t* buffer, size_t len);

    // control
    void stop();

    I2SMode mode() const;

private:
    i2s_port_t _port;
    I2SMode _mode = I2SMode::NONE;

    void _installDriver(const i2s_config_t& config);
};