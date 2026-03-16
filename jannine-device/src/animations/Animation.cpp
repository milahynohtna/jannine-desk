#include "../../include/animation/Animation.h"
#include <Arduino.h>

// Animation tidak murni interface (Engine)
// Dia punya:
//  *state machine
//  *timing logic
//  *default behavior
// Artinya:
//  Animation = abstract base class + shared implementation


// --- Setter frame ---
void Animation::setIntro(std::vector<FrameDuration> frames) { _intro = std::move(frames); }
void Animation::setLoop(std::vector<FrameDuration> frames)  { _loop  = std::move(frames); }
void Animation::setOutro(std::vector<FrameDuration> frames) { _outro = std::move(frames); }

// --- Reset animation ---
void Animation::reset() {
    _index = 0;
    _elapsedMs = 0;
    _loopEnabled = true;
    _lastTickMs = 0;

    // gunakan array helper untuk mencari phase pertama dengan frames
    const Phase phases[] = { Phase::Intro, Phase::Loop, Phase::Outro };
    for (auto p : phases) {
        const std::vector<FrameDuration>* frames = getPhaseFrames(p);
        if (frames && !frames->empty()) {
            _phase = p;
            _active = frames;
            return;
        }
    }


    _phase = Phase::Finished;
    _active = nullptr;
}

// --- Request finish ---
void Animation::stopLoop() {
    if (_phase == Phase::Loop) _loopEnabled = false;
}

void Animation::requestFinish() {
    stopLoop();
}

// --- Query state ---
bool Animation::isFinished() const { return _phase == Phase::Finished; }

const ImageDescriptor& Animation::currentFrame() const {
    static ImageDescriptor dummy{};
    if (!_active || _active->empty() || _index >= _active->size()) return dummy;
    return (*_active)[_index].frame;
}

// --- Tick & update ---
void Animation::tick() {
    if (_phase == Phase::Finished) return;

    uint32_t now = millis();
    if (_lastTickMs == 0) { _lastTickMs = now; return; }

    uint32_t dt = now - _lastTickMs;
    _lastTickMs = now;

    update(dt);
}

void Animation::update(uint32_t deltaMs) {
    if (_phase == Phase::Finished || !_active || _active->empty()) return;

    _elapsedMs += deltaMs;
    if (_elapsedMs >= (*_active)[_index].durationMs) {
        _elapsedMs = 0;
        advanceFrame();
    }
}

// --- Helper: mapping phase ke frames ---
const std::vector<FrameDuration>* Animation::getPhaseFrames(Phase p) const {
    switch (p) {
        case Phase::Intro:  return &_intro;
        case Phase::Loop:   return &_loop;
        case Phase::Outro:  return &_outro;
        default:            return nullptr;
    }
}

// --- Advance frame generik ---
void Animation::advanceFrame() {
    _index++;
    if (_active && _index < _active->size()) return;

    // Phase transition generik
    Phase next;


    switch (_phase) {
        case Phase::Intro:
            next = _loopEnabled && !_loop.empty() ? Phase::Loop
                 : !_outro.empty() ? Phase::Outro
                 : Phase::Finished;
            break;

        case Phase::Loop:
            next = _loopEnabled ? Phase::Loop
                 : !_outro.empty() ? Phase::Outro
                 : Phase::Finished;
            break;

        case Phase::Outro:
            next = Phase::Finished;
            break;

        default:
            next = Phase::Finished;
    }

    _phase = next;
    _active = getPhaseFrames(_phase);
    _index = 0;
}

