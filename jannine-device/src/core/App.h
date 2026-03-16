#pragma once

#include <Arduino.h>

#include "../infrastructure/drivers/LovyanDriver.h"
#include "../components/Display.h"

#include "../infrastructure/network/WiFiManager.h"
#include "../infrastructure/network/WebSocketClient.h"
#include "../infrastructure/network/NetworkSetup.h"
#include "../config/WebSocketConfig.h"

#include "../runtime/RobotRuntime.h"
#include "../conversation/ConversationEngine.h"

#include "../infrastructure/serial_debugging/SerialInputHandler.h"
#include "../infrastructure/setup/SystemSetup.h"

class App {
public:

    App();

    void setup();
    void loop();

private:

    LovyanDriver _driver;
    Display _display;

    WiFiManager _wifiManager;
    WebSocketClient _wsClient;
    NetworkSetup _networkSetup;

    SerialInputHandler _serial;

    RobotRuntime _runtime;
    ConversationEngine _conversation;

    SystemSetup _setup;
};