#pragma once

#include <Arduino.h>

#include "components/Display.h"
#include "runtime/RobotRuntime.h"
#include "conversation/ConversationEngine.h"

#include "infrastructure/network/NetworkSetup.h"
#include "infrastructure/network/WebSocketClient.h"

#include "config/WebSocketConfig.h"

class SystemSetup {
public:

    SystemSetup(
        Display& display,
        RobotRuntime& runtime,
        ConversationEngine& conversation,
        WebSocketClient& wsClient,
        NetworkSetup& network
    );

    void run();

private:

    Display& _display;
    RobotRuntime& _runtime;
    ConversationEngine& _conversation;

    WebSocketClient& _wsClient;
    NetworkSetup& _network;




    void _setupWebSocketCallbacks();
    void _setupNetwork();
};