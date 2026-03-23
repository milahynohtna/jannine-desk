#include "Microphone.h"

// ──────────────────────────────────────────────────────
// Constructor
// ──────────────────────────────────────────────────────
Microphone::Microphone(
    I2SDriver& driver,
    const MicrophonePins& pins,
    const MicrophoneSettings& settings
)
    : _driver(driver),
      _pins(pins),
      _settings(settings)
{
}

// ──────────────────────────────────────────────────────
// INIT
// ──────────────────────────────────────────────────────
void Microphone::init() {
    _driver.beginMic(_pins, _settings);
}

// ──────────────────────────────────────────────────────
// START
// ──────────────────────────────────────────────────────
void Microphone::start()
{
    if (_started) return;

    _driver.stop();   // 🔥 IMPORTANT: ensure clean state

    _driver.beginMic(MicrophoneConfig::PINS, MicrophoneConfig::SETTINGS);

    _started = true;
}

// ──────────────────────────────────────────────────────
// STOP
// ──────────────────────────────────────────────────────
void Microphone::stop() {
    _driver.stop();
    _started = false;
}

// ──────────────────────────────────────────────────────
// READ
// ──────────────────────────────────────────────────────
size_t Microphone::read(uint8_t* buffer, size_t len) {
    if (!_started) return 0;
    return _driver.read(buffer, len);
}

// ──────────────────────────────────────────────────────
// META
// ──────────────────────────────────────────────────────
uint32_t Microphone::sampleRate() const {
    return _settings.sample_rate;
}

uint16_t Microphone::bufferSize() const {
    return _settings.buffer_size;
}