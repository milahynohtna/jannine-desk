#pragma once

#include "animation/Animation.h"

// frames
#include "images/listening_0002.h"
#include "images/listening_0014.h"
#include "images/listening_0016.h"
#include "images/listening_0020.h"
#include "images/listening_0021.h"
#include "images/listening_0022.h"
#include "images/listening_0030.h"
#include "images/listening_0034.h"
#include "images/listening_0036.h"
#include "images/listening_0037.h"

class ListeningAnimation : public Animation {
public:
    ListeningAnimation() {

        setIntro({
            { LISTENING_0002, 1200 },
            {LISTENING_0014, 200},
            {LISTENING_0016, 400},
            {LISTENING_0020, 100},
        });


        setLoop({
            {LISTENING_0021, 200},
            {LISTENING_0022, 500},
            {LISTENING_0030, 400},
            {LISTENING_0034, 300},
            {LISTENING_0036, 200},
            {LISTENING_0034, 300},
            {LISTENING_0030, 400},
            {LISTENING_0022, 500},
        });

        setOutro({
            { LISTENING_0037, 1000 }
        });

        reset();   // 🔥 WAJIB

    }

};
