#pragma once

#include "animation/Animation.h"

// frames
#include "images/idle_0011.h"
#include "images/idle_0016.h"
#include "images/idle_0017.h"


class IdleAnimation : public Animation {
public:
    IdleAnimation() {

        setIntro({
            { IDLE_0011, 500 },
        });
        setLoop({
            {IDLE_0011, 2500},
            {IDLE_0016, 80},
            {IDLE_0017, 80},
            {IDLE_0016, 80},
        });
        setOutro({
            { IDLE_0011, 1000 }
        });
        reset();   // 🔥 WAJIB
    }

};
