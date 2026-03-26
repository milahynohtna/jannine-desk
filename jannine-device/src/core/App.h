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

#include "../infrastructure/setup/SystemSetup.h"

// 🔊 audio
#include "../infrastructure/drivers/I2SDriver.h"
#include "components//Microphone.h"

class App {
public:

    App();

    void setup();
    void loop();

private:

    // UI
    LovyanDriver _driver;
    Display _display;

    // Network
    WiFiManager _wifiManager;
    WebSocketClient _wsClient;
    NetworkSetup _networkSetup;

    // Audio
    I2SDriver _i2s;
    Microphone _microphone;


    // Core
    RobotRuntime _runtime;
    ConversationEngine _conversation;

    // Setup orchestrator
    SystemSetup _setup;
};