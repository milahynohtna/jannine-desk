#include "I2SDriver.h"

void I2SDriver::begin(
    const AudioPins& pins,
    const AudioSettings& settings
)
{
    i2s_config_t config = {
        .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate          = settings.sample_rate,
        .bits_per_sample      = settings.bits_per_sample,
        .channel_format       = settings.channel_format,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count        = settings.dma_buf_count,
        .dma_buf_len          = settings.dma_buf_len,
        .use_apll             = settings.use_apll,
        .tx_desc_auto_clear   = true,
        .fixed_mclk           = 0
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num   = pins.bclk,
        .ws_io_num    = pins.lrc,
        .data_out_num = pins.dout,
        .data_in_num  = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(_port, &config, 0, nullptr);
    i2s_set_pin(_port, &pin_config);
    i2s_zero_dma_buffer(_port);
}

void I2SDriver::write(const uint8_t* data, size_t len)
{
    size_t written;
    i2s_write(_port, data, len, &written, portMAX_DELAY);
}

void I2SDriver::stop()
{
    i2s_zero_dma_buffer(_port);     // flush sisa audio di DMA agar tidak noise
    i2s_driver_uninstall(_port);    // lepas driver sepenuhnya
}