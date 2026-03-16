#pragma once

#include <Arduino.h>
#include "../runtime/RobotRuntime.h"
#include "infrastructure/network/WebSocketClient.h"

class ConversationEngine {
public:

    ConversationEngine(
        WebSocketClient& ws,
        RobotRuntime& runtime
    );

    void sendPrompt(const String& prompt);

    void onToken(const String& token);
    void onDone();

private:

    WebSocketClient& _ws;
    RobotRuntime& _runtime;

    String _tokenBuffer;
};