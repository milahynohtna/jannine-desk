#pragma once

#include <Arduino.h>
#include "../runtime/RobotRuntime.h"
#include "infrastructure/network/WebSocketClient.h"
#include "components/Microphone.h"

class ConversationEngine {
public:

    ConversationEngine(
        WebSocketClient& ws,
        RobotRuntime& runtime,
        Microphone& mic
    );

    void startListening();
    void startSpeaking();
    void update();

    void onToken(const String& token);
    void onDone();

private:

    WebSocketClient& _ws;
    RobotRuntime& _runtime;
    Microphone& _mic;

    String _tokenBuffer;

    static constexpr size_t BUFFER_SIZE = 1024;
    uint8_t _buffer[BUFFER_SIZE];

    bool _streaming = false;

    // 🔥 timer listening
    unsigned long _listenStart = 0;
    static constexpr unsigned long LISTEN_DURATION_MS = 7000;

    // 🔥 sync playback
    bool _waitingAudioFinish = false;
    bool _isSpeaking = false;
    bool _speakingTriggered = false; // 🔥 NEW (anti burst)
};