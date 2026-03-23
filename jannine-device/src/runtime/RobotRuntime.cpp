#include "RobotRuntime.h"

unsigned long _lastPlayTime = 0;

RobotRuntime::RobotRuntime(Display& display, I2SDriver& driver)
: _display(display)
, _audio(driver)   // 🔥 INI KUNCI
{}

void RobotRuntime::begin()
{
    _state.set(RobotState::Idle);
}

Audio& RobotRuntime::audio()
{
    return _audio;
}

// ──────────────────────────────────────────────────────
// STATE CONTROL
// ──────────────────────────────────────────────────────
void RobotRuntime::setListening()
{
    _audio.clear();
    _state.set(RobotState::Listening);
}

bool _audioStarted = false;

void RobotRuntime::setSpeaking()
{
    if (_state.is(RobotState::Talking)) return;
    _audio.begin(1024 * 1024);
    _state.set(RobotState::Talking);
    _audioStarted = false; // reset, tunggu sampai benar-benar ada data
}

void RobotRuntime::setIdle()
{
    _state.set(RobotState::Idle);
}

// ──────────────────────────────────────────────────────
// UPDATE LOOP
// ──────────────────────────────────────────────────────
void RobotRuntime::update()
{
    _updateAudioState();
    _updateAnimation();
}

// ──────────────────────────────────────────────────────
// AUDIO STATE SYNC
// ──────────────────────────────────────────────────────
void RobotRuntime::_updateAudioState()
{
    if (_state.is(RobotState::Talking))
    {
        if (_audio.isPlaying()) {
            _audioStarted = true;
            _lastPlayTime = millis(); // ✅ catat kapan terakhir ada audio
        }

        // ✅ tunggu minimal 200ms setelah stop sebelum ganti state
        if (_audioStarted && !_audio.isPlaying() &&
            (millis() - _lastPlayTime > 200))
        {
            _audioStarted = false;
            _state.set(RobotState::Idle);
        }
    }
}

// ──────────────────────────────────────────────────────
// ANIMATION
// ──────────────────────────────────────────────────────
void RobotRuntime::_updateAnimation()
{
    Animation* anim = nullptr;

    switch (_state.robotState)
    {
        case RobotState::Idle:       anim = &_idle; break;
        case RobotState::Listening:  anim = &_listening; break;
        case RobotState::Talking:    anim = &_talking; break;
        default: return;
    }

    if (!anim) return;

    anim->tick();
    _display.showAnimation(*anim);
}