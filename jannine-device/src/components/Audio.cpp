#include "Audio.h"
#include <Arduino.h>

Audio::Audio() = default;

bool Audio::begin(size_t bufferSize) {
    if (!_buffer.begin(bufferSize)) {
        Serial.println("[Audio] Ring buffer alloc failed");
        return false;
    }

    _task.begin(&_buffer);
    Serial.println("[Audio] Task running on Core0");
    return true;
}

void Audio::stop() {
    _task.stop();
    _buffer.clear();
}

bool Audio::write(const uint8_t* data, size_t length) {
    if (_buffer.free() < length) {
        Serial.println("[Audio] Buffer full, dropping packet");
        return false;
    }
    _buffer.write(data, length);
    return true;
}

void Audio::clear() {
    _buffer.clear();
}

bool Audio::isPlaying() const {
    return _task.isPlaying();
}

size_t Audio::freeBytes() const {
    return _buffer.free();
}