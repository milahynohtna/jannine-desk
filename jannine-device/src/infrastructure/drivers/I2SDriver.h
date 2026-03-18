#pragma once

#include "driver/i2s.h"
#include <cstddef>
#include <cstdint>

// forward declaration (biar tidak tight coupling)
struct AudioPins;
struct AudioSettings;

struct MicrophonePins;
struct MicrophoneSettings;

// ──────────────────────────────────────────────────────
// I2SDriver
// tanggung jawab:
//   - init I2S (TX / RX)
//   - write (speaker)
//   - read (mic)
//   - stop driver
// ──────────────────────────────────────────────────────
class I2SDriver {
public:
    explicit I2SDriver(i2s_port_t port = I2S_NUM_0);

    // ───────────── TX (Speaker) ─────────────
    void begin(
        const AudioPins& pins,
        const AudioSettings& settings
    );

    void write(const uint8_t* data, size_t len);

    // ───────────── RX (Microphone) ─────────────
    void beginMic(
        const MicrophonePins& pins,
        const MicrophoneSettings& settings
    );

    size_t read(uint8_t* buffer, size_t len);

    // ───────────── CONTROL ─────────────
    void stop();

private:
    i2s_port_t _port;
};