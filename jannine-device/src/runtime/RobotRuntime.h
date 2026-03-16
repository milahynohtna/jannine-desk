#pragma once

#include "../components/Display.h"
#include "../components/Audio.h"
#include "../core/AppState.h"

#include "../animations/IdleAnimation.h"
#include "../animations/ListeningAnimation.h"
#include "../animations/TalkingAnimation.h"

class RobotRuntime {
public:

    RobotRuntime(Display& display);

    void begin();
    void update();

    Audio& audio();

    void setListening();
    void setIdle();

private:

    Display& _display;
    Audio _audio;

    AppState _state;

    IdleAnimation _idle;
    ListeningAnimation _listening;
    TalkingAnimation _talking;

    void _updateAnimation();
    void _updateAudioState();
};