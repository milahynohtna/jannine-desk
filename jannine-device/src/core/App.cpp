#include "App.h"


App::App()
: _display(&_driver)
, _networkSetup(_wifiManager, _wsClient)
, _runtime(_display)
, _conversation(_wsClient, _runtime)
, _setup(
    _display,
    _runtime,
    _conversation,
    _wsClient,
    _networkSetup,
    _serial
)
{}

void App::setup()
{
    _setup.run();
}

void App::loop()
{
    _wsClient.poll();
    _networkSetup.checkAndReconnect(RECONNECT_DELAY_MS);

    _serial.checkInput();

    _runtime.update();
}