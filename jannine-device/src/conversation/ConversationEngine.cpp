#include "ConversationEngine.h"

ConversationEngine::ConversationEngine(
    WebSocketClient& ws,
    RobotRuntime& runtime,
    Microphone& mic
)
: _ws(ws)
, _runtime(runtime)
, _mic(mic)
{}

// ──────────────────────────────────────────────────────
// LISTENING
// ──────────────────────────────────────────────────────
void ConversationEngine::startListening()
{
    if (_streaming) return;

    Serial.println("\n[STATE] -> LISTENING (3s)");

    _tokenBuffer = "";

    _runtime.audio().stop();
    delay(20);

    _mic.init();
    _mic.start();

    _runtime.setListening();

    _streaming = true;
    _waitingAudioFinish = false;

    // 🔥 reset speaking state
    _isSpeaking = false;
    _speakingTriggered = false;

    _listenStart = millis();
}

// ──────────────────────────────────────────────────────
// SPEAKING
// ──────────────────────────────────────────────────────
void ConversationEngine::startSpeaking()
{
    // 🔥 HARD GUARD (anti spam)
    if (_speakingTriggered) return;

    _speakingTriggered = true; // 🔥 lock immediately

    Serial.println("[STATE] -> SPEAKING");

    _mic.stop();
    delay(20);

    _runtime.setSpeaking();

    _streaming = false;
    _isSpeaking = true;
}

// ──────────────────────────────────────────────────────
// LOOP
// ──────────────────────────────────────────────────────
void ConversationEngine::update()
{
    if (!_ws.isConnected()) return;

    // ───────── LISTEN ─────────
    if (_streaming)
    {
        size_t n = _mic.read(_buffer, BUFFER_SIZE);

        if (n > 0) {
            _ws.sendBytes(_buffer, n);
        }

        if (millis() - _listenStart >= LISTEN_DURATION_MS)
        {
            Serial.println("[LISTEN] timeout → stop mic");

            _mic.stop();
            delay(20);

            _streaming = false;

            _ws.sendText("{\"type\":\"audio_end\"}");

            Serial.println("[STATE] -> WAITING RESPONSE");
        }
    }

    // ───────── WAIT AUDIO FINISH ─────────
    if (_waitingAudioFinish)
    {
        if (!_runtime.audio().isPlaying())
        {
            Serial.println("[AUDIO] playback finished → LISTENING");

            _waitingAudioFinish = false;
            startListening();
        }
    }
}

// ──────────────────────────────────────────────────────
// TOKEN
// ──────────────────────────────────────────────────────
void ConversationEngine::onToken(const String& token)
{
    _tokenBuffer += token;
}

// ──────────────────────────────────────────────────────
// DONE
// ──────────────────────────────────────────────────────
void ConversationEngine::onDone()
{
    Serial.print("Jannine: ");
    Serial.println(_tokenBuffer);

    Serial.println("[Stream Finished]");

    _tokenBuffer = "";

    _waitingAudioFinish = true;

    // 🔥 release speaking AFTER done
    _isSpeaking = false;
}