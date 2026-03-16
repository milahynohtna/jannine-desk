#pragma once

#include "driver/i2s.h"
#include "cstdint"

// ──────────────────────────────────────────────────────
// AudioPins — tanggung jawab tunggal:
//   menyimpan konfigurasi pin hardware I2S
// ──────────────────────────────────────────────────────
struct AudioPins {
    int16_t bclk;
    int16_t lrc;
    int16_t dout;
};

// ──────────────────────────────────────────────────────
// AudioSettings — tanggung jawab tunggal:
//   menyimpan konfigurasi parameter audio I2S
// ──────────────────────────────────────────────────────
struct AudioSettings {
    uint32_t              sample_rate;
    i2s_bits_per_sample_t bits_per_sample;
    i2s_channel_fmt_t     channel_format;
    uint8_t               dma_buf_count;
    uint16_t              dma_buf_len;
    bool                  use_apll;
    uint16_t              buffer_size;
};

// ──────────────────────────────────────────────────────
// AudioConfig — tanggung jawab tunggal:
//   menyediakan nilai default hardware untuk project ini
// ──────────────────────────────────────────────────────
namespace AudioConfig {

    constexpr AudioPins PINS = {
        .bclk = 38,
        .lrc  = 39,
        .dout = 40
    };

    constexpr AudioSettings SETTINGS = {
        .sample_rate     = 20000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format  = I2S_CHANNEL_FMT_ONLY_LEFT,
        .dma_buf_count   = 8,
        .dma_buf_len     = 512,
        .use_apll        = true,
        .buffer_size     = 4096
    };

}
