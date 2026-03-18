#pragma once
#include "../../include/display/IDisplayDriver.h"
#include "images/ImageDescriptor.h"
#include "animation/Animation.h"



class Display {
private:
    IDisplayDriver* _driver;

public:
    explicit Display(IDisplayDriver* driver);

    void begin() const;
    void clear(uint16_t color) const;


    void showImage(const ImageDescriptor& img) const;
    void showAnimation(const Animation& animation) const;

    void drawText(const char* text, int x, int y, uint16_t color) const;
    void setRotation(int rotation) const;
};
