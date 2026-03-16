#pragma once
#include "cstdint"

struct ImageDescriptor {
    int x;
    int y;
    int width;
    int height;
    const uint16_t* data;
};
