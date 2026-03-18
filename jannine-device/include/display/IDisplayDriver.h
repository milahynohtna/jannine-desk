#pragma once


#include "images/ImageDescriptor.h"
#include "animation/Animation.h"

class IDisplayDriver {
public:
    virtual ~IDisplayDriver() = default;

    virtual bool init() = 0;
    virtual void setRotation(int rotation) = 0;
    virtual void clear(uint16_t color) = 0;

    // ⬇️ INI WAJIB ADA
    virtual void showImage(const ImageDescriptor& img) = 0;
    virtual void showAnimation(const Animation& animation) = 0;


    virtual void drawText(
        const char* text,
        int x,
        int y,
        uint16_t color
    ) = 0;


};
