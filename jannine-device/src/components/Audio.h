#pragma once

#include "infrastructure/audio/AudioRingBuffer.h"
#include "infrastructure/audio/AudioTask.h"

class Audio {
public:
    Audio();

    // Lifecycle
    bool begin(size_t bufferSize = 512 * 1024);
    void stop();

    // Data
    bool write(const uint8_t* data, size_t length);
    void clear();

    // State
    bool isPlaying() const;
    size_t freeBytes() const;

private:
    AudioRingBuffer _buffer;
    AudioTask       _task;
};