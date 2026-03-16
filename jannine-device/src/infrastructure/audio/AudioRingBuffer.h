#pragma once
#include <Arduino.h>
#include "esp_heap_caps.h"

class AudioRingBuffer {
public:
    bool begin(size_t size)
    {
        _size = size;

        _buffer = (uint8_t*) heap_caps_malloc(
            size,
            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT
        );

        if (!_buffer) {
            Serial.println("[AudioRingBuffer] PSRAM allocation failed");
            return false;
        }

        _write = 0;
        _read  = 0;

        return true;
    }

    inline size_t available() const
    {
        if (_write >= _read)
            return _write - _read;
        return _size - (_read - _write);
    }

    inline size_t free() const
    {
        return _size - available() - 1;
    }

    void write(const uint8_t* data, size_t len)
    {
        if (len > free())
            return;

        size_t first = min(len, _size - _write);

        memcpy(_buffer + _write, data, first);

        if (len > first)
            memcpy(_buffer, data + first, len - first);

        _write = (_write + len) % _size;
    }

    size_t read(uint8_t* out, size_t len)
    {
        size_t avail = available();
        if (avail == 0) return 0;

        if (len > avail)
            len = avail;

        size_t first = min(len, _size - _read);

        memcpy(out, _buffer + _read, first);

        if (len > first)
            memcpy(out + first, _buffer, len - first);

        _read = (_read + len) % _size;

        return len;
    }

    void clear()
    {
        _read  = 0;
        _write = 0;
    }

private:
    uint8_t* _buffer = nullptr;
    size_t _size = 0;

    volatile size_t _write = 0;
    volatile size_t _read  = 0;
};