#pragma once
#include <Arduino.h>
#include "esp_heap_caps.h"
#include "freertos/semphr.h"

class AudioRingBuffer {
public:
    bool begin(size_t size)
    {
        _mutex = xSemaphoreCreateMutex();
        if (!_mutex) {
            Serial.println("[AudioRingBuffer] Mutex creation failed");
            return false;
        }

        _size = size;

        // 🔥 coba PSRAM dulu
        _buffer = (uint8_t*) heap_caps_malloc(
            size,
            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT
        );

        // 🔥 fallback ke internal RAM kalau PSRAM gagal
        if (!_buffer) {
            Serial.println("[AudioRingBuffer] PSRAM failed, trying internal RAM...");
            _buffer = (uint8_t*) heap_caps_malloc(
                size,
                MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT
            );
        }

        if (!_buffer) {
            Serial.println("[AudioRingBuffer] ALL allocation failed!");
            return false;
        }

        Serial.printf("[AudioRingBuffer] Allocated %d bytes\n", size);

        _write = 0;
        _read  = 0;

        return true;
    }

    size_t available() const
    {
        if (!_mutex) return 0;  // 🔥 guard

        xSemaphoreTake(_mutex, portMAX_DELAY);
        size_t val = _availableUnsafe();
        xSemaphoreGive(_mutex);
        return val;
    }

    size_t free() const
    {
        if (!_mutex) return 0;  // 🔥 guard

        xSemaphoreTake(_mutex, portMAX_DELAY);
        size_t val = _size - _availableUnsafe() - 1;
        xSemaphoreGive(_mutex);
        return val;
    }

    void write(const uint8_t* data, size_t len)
    {
        if (!_mutex) return;  // 🔥 guard


        xSemaphoreTake(_mutex, portMAX_DELAY);

        if (len > (_size - _availableUnsafe() - 1)) {
            xSemaphoreGive(_mutex);
            return;
        }

        size_t first = min(len, _size - _write);
        memcpy(_buffer + _write, data, first);

        if (len > first)
            memcpy(_buffer, data + first, len - first);

        _write = (_write + len) % _size;

        xSemaphoreGive(_mutex);
    }

    size_t read(uint8_t* out, size_t len)
    {
        if (!_mutex) return 0;  // 🔥 guard

        xSemaphoreTake(_mutex, portMAX_DELAY);

        size_t avail = _availableUnsafe();
        if (avail == 0) {
            xSemaphoreGive(_mutex);
            return 0;
        }

        if (len > avail) len = avail;

        size_t first = min(len, _size - _read);
        memcpy(out, _buffer + _read, first);

        if (len > first)
            memcpy(out + first, _buffer, len - first);

        _read = (_read + len) % _size;

        xSemaphoreGive(_mutex);
        return len;
    }

    void clear()
    {
        if (!_mutex) return;  // 🔥 guard

        xSemaphoreTake(_mutex, portMAX_DELAY);
        _read  = 0;
        _write = 0;
        xSemaphoreGive(_mutex);
    }

private:
    inline size_t _availableUnsafe() const
    {
        if (_write >= _read)
            return _write - _read;
        return _size - (_read - _write);
    }

    uint8_t*          _buffer = nullptr;
    size_t            _size   = 0;
    size_t            _write  = 0;
    size_t            _read   = 0;
    SemaphoreHandle_t _mutex  = nullptr;
};