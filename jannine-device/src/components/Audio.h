#pragma once

#include "infrastructure/audio/AudioRingBuffer.h"
#include "infrastructure/audio/AudioTask.h"



class Audio {
public:
    explicit Audio(I2SDriver& driver);  // 🔥 wajib inject

    bool begin(size_t bufferSize = 8192);
    void stop();

    bool write(const uint8_t* data, size_t length);
    void clear();

    bool isPlaying() const;
    size_t freeBytes() const;

private:
    AudioRingBuffer _buffer;
    AudioTask _task;  // sekarang butuh driver dari ctor

    bool _initialized = false;
};