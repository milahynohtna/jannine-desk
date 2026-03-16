#pragma once

#include <atomic>

#include "AudioRingBuffer.h"
#include "../drivers/I2SDriver.h"

class AudioTask {
public:

    AudioTask()
        : _buffer(nullptr)
    {}

    void begin(AudioRingBuffer* buffer)
    {
        _buffer = buffer;
        _i2s.begin();

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

    void stop()
    {
        if (_taskHandle)
        {
            vTaskDelete(_taskHandle);
            _taskHandle = nullptr;
        }
        _i2s.stop();
        _playing = false;
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

        constexpr size_t CHUNK = 1024;
        uint8_t temp[CHUNK];

        for (;;)
        {
            size_t read = self->_buffer->read(temp, CHUNK);

            if (read > 0)
            {
                self->_playing.store(true, std::memory_order_release);
                self->_i2s.write(temp, read);
            }
            else
            {
                self->_playing.store(false, std::memory_order_release);
                vTaskDelay(1);
            }
        }
    }

    AudioRingBuffer*          _buffer     = nullptr;
    I2SDriver                 _i2s;
    TaskHandle_t              _taskHandle = nullptr;
    std::atomic<bool>         _playing    { false };  // ← ganti volatile bool
};