#include <Arduino.h>
#include "core/App.h"

static App app;

void setup() {
    app.setup();
}

void loop() {
    app.loop();
}
