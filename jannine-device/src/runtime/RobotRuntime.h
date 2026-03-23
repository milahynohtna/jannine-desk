#pragma once

#include "../components/Display.h"
#include "../components/Audio.h"
#include "../core/AppState.h"
#include "infrastructure/drivers/I2SDriver.h"   // 🔥 tambahkan ini

#include "../animations/IdleAnimation.h"
#include "../animations/ListeningAnimation.h"
#include "../animations/TalkingAnimation.h"

class RobotRuntime {
public:

    RobotRuntime(Display& display, I2SDriver& driver); // 🔥 tambah driver

    void begin();
    void update();

    Audio& audio();

    void setListening();
    void setSpeaking();
    void setIdle();

private:

    Display& _display;
    Audio _audio;   // sekarang valid karena di-init di ctor

    AppState _state;

    IdleAnimation _idle;
    ListeningAnimation _listening;
    TalkingAnimation _talking;

    void _updateAnimation();
    void _updateAudioState();
};