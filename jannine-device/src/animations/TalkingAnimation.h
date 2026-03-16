#pragma once

#include "animation/Animation.h"

// frames
#include "images/talking_0001.h"
#include "images/talking_0011.h"
#include "images/talking_0012.h"
#include "images/talking_0013.h"
#include "images/talking_0016.h"
#include "images/talking_0017.h"
#include "images/talking_0018.h"
#include "images/talking_0025.h"
#include "images/talking_0036.h"




class TalkingAnimation : public Animation {
public:
    TalkingAnimation() {

        setIntro({
            {TALKING_0001, 50},
            { TALKING_0011, 20 },
            { TALKING_0012, 20 },
            { TALKING_0013, 20 },
            { TALKING_0016, 20 },
        });


        setLoop({
            {TALKING_0017, 100},
            {TALKING_0018, 100},
            {TALKING_0025, 100},
            {TALKING_0036, 100},
            {TALKING_0018, 100},
        });

        setOutro({
            { TALKING_0001, 1000 },
            {TALKING_0012, 500},
        });

        reset();   // 🔥 WAJIB

    }

};
