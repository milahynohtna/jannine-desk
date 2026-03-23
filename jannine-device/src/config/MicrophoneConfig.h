#pragma once

#include "driver/i2s.h"
#include <cstdint>

// ──────────────────────────────────────────────────────
// MicPins — khusus untuk input (INMP441)
// ──────────────────────────────────────────────────────
struct MicrophonePins {
    int16_t bclk;
    int16_t lrc;
    int16_t din;   // data dari mic ke ESP32
};

// ──────────────────────────────────────────────────────
// MicSettings — konfigurasi audio untuk mic
// ──────────────────────────────────────────────────────
struct MicrophoneSettings {
    uint32_t              sample_rate;
    i2s_bits_per_sample_t bits_per_sample;
    i2s_channel_fmt_t     channel_format;
    uint8_t               dma_buf_count;
    uint16_t              dma_buf_len;
    bool                  use_apll;
    uint16_t              buffer_size;
};

// ──────────────────────────────────────────────────────
// MicConfig — default untuk INMP441
// ──────────────────────────────────────────────────────
namespace MicrophoneConfig {

    constexpr MicrophonePins PINS = {
        .bclk = 14,
        .lrc  = 18,
        .din  = 10   //
    };

    constexpr MicrophoneSettings SETTINGS = {
        .sample_rate     = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // 🔥 ganti dari 32BIT
        .channel_format  = I2S_CHANNEL_FMT_ONLY_LEFT,
        .dma_buf_count   = 8,
        .dma_buf_len     = 256,
        .use_apll        = false,
        .buffer_size     = 4096
    };

}