#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <stdint.h>   // ← tambahkan ini



struct DisplayPins {
    int16_t sclk;   // -128 to 127, support -1 for unused
    int16_t mosi;
    int16_t dc;
    int16_t cs;
    int16_t rst;
};

struct DisplaySettings {
    uint16_t panel_width;      // 0-65535
    uint16_t panel_height;     // 0-65535
    bool invert;               // 1 byte (compiler might pad to 4)
    bool rgb_order;            // 1 byte
    uint8_t rotation;          // 0-3 only
    uint32_t spi_freq_write;   // Support up to 4.2GHz
    uint32_t spi_freq_read;    // Support up to 4.2GHz
};

// Default config untuk ESP32-S3 + GC9A01
namespace DisplayConfig {
    constexpr DisplayPins PINS = {
        .sclk = 15,   // SCL
        .mosi = 7,   // SDA
        .dc = 6,
        .cs = 5,
        .rst = 4
    };

    constexpr DisplaySettings SETTINGS = {
        .panel_width = 240,
        .panel_height = 240,
        .invert = true,
        .rgb_order = false,
        .rotation = 0,
        .spi_freq_write = 40000000,
        .spi_freq_read = 16000000
    };
}

#endif