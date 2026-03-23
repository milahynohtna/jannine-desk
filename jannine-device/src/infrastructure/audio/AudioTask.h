#pragma once
#include <atomic>
#include "AudioRingBuffer.h"
#include "../drivers/I2SDriver.h"
#include "config/AudioConfig.h"

class AudioTask {
public:
    AudioTask(I2SDriver& driver)
        : _i2s(driver), _buffer(nullptr)
    {}

    void begin(AudioRingBuffer* buffer)
    {
        _buffer = buffer;

        xTaskCreatePinnedToCore(
            _taskEntry,
            "AudioTask",
            4096,
            this,
            3,
            &_taskHandle,
            0
        );
    }

    void startI2S()
    {
        _i2s.stop();
        _i2s.begin(AudioConfig::PINS, AudioConfig::SETTINGS);
    }

    void stop()
    {
        if (_taskHandle)
        {
            vTaskDelete(_taskHandle);
            _taskHandle = nullptr;
        }

        _i2s.stop();
        _playing = false;
        _lastDataTime = 0;
    }

    bool isPlaying() const
    {
        return _playing;
    }

private:
    static void _taskEntry(void* arg)
    {
        auto* self = static_cast<AudioTask*>(arg);

        if (!self->_buffer)
        {
            vTaskDelete(nullptr);
            return;
        }

        constexpr size_t   CHUNK    = 1024;
        constexpr uint32_t GRACE_MS = 800;  // toleransi jeda antar-paket network
        uint8_t temp[CHUNK];

        for (;;)
        {
            size_t read = self->_buffer->read(temp, CHUNK);

            if (read > 0)
            {
                self->_lastDataTime = millis();
                self->_playing.store(true);
                self->_i2s.write(temp, read);
            }
            else
            {
                // hanya set false setelah GRACE_MS tidak ada data
                if (self->_lastDataTime > 0 &&
                    (millis() - self->_lastDataTime > GRACE_MS))
                {
                    self->_playing.store(false);
                }
                vTaskDelay(1);
            }
        }
    }

    AudioRingBuffer*  _buffer       = nullptr;
    I2SDriver&        _i2s;
    TaskHandle_t      _taskHandle   = nullptr;
    std::atomic<bool> _playing      { false };
    uint32_t          _lastDataTime = 0;
};