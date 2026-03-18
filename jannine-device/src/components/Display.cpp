#include "Display.h"


Display::Display(IDisplayDriver* driver)
    : _driver(driver) {}

void Display::begin() const {
    _driver->init();
}

void Display::clear(uint16_t color) const {
    _driver->clear(color);
}

void Display::showImage(const ImageDescriptor& img) const {
    _driver->showImage(img);
}

void Display::drawText(
    const char* text,
    int x,
    int y,
    uint16_t color
) const {
    _driver->drawText(text, x, y, color);
}

void Display::showAnimation(const Animation& animation) const{
    _driver->showAnimation(animation);
}

void Display::setRotation(int rotation) const {
    _driver->setRotation(rotation);
}
