#include "RobotRuntime.h"

RobotRuntime::RobotRuntime(Display& display)
: _display(display)
{}

void RobotRuntime::begin()
{
    _audio.begin();
    _state.set(RobotState::Idle);
}

Audio& RobotRuntime::audio()
{
    return _audio;
}

void RobotRuntime::setListening()
{
    _audio.clear();
    _state.set(RobotState::Listening);
}

void RobotRuntime::setIdle()
{
    _state.set(RobotState::Idle);
}

void RobotRuntime::update()
{
    _updateAudioState();
    _updateAnimation();
}

void RobotRuntime::_updateAudioState()
{
    if (_audio.isPlaying() && !_state.is(RobotState::Talking))
        _state.set(RobotState::Talking);

    if (!_audio.isPlaying() && _state.is(RobotState::Talking))
        _state.set(RobotState::Idle);
}

void RobotRuntime::_updateAnimation()
{
    Animation* anim = nullptr;

    switch(_state.robotState)
    {
        case RobotState::Idle: anim = &_idle; break;
        case RobotState::Listening: anim = &_listening; break;
        case RobotState::Talking: anim = &_talking; break;
        default: return;
    }

    anim->tick();
    _display.showAnimation(*anim);
}