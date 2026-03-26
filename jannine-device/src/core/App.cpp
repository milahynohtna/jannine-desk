#include "App.h"

App::App()
: _display(&_driver)
, _networkSetup(_wifiManager, _wsClient)

// 🔥 SINGLE I2S DRIVER
, _i2s(I2S_NUM_0)
, _microphone(_i2s)

// core
, _runtime(_display, _i2s)
, _conversation(_wsClient, _runtime, _microphone)

// setup
, _setup(
    _display,
    _runtime,
    _conversation,
    _wsClient,
    _networkSetup
)
{}

void App::setup() {
    _setup.run();
}

void App::loop() {
    _wsClient.poll();
    _networkSetup.checkAndReconnect(RECONNECT_DELAY_MS);

    _runtime.update();

    // 🔥 WAJIB
    _conversation.update();
}