#include "ConversationEngine.h"

ConversationEngine::ConversationEngine(
    WebSocketClient& ws,
    RobotRuntime& runtime
)
: _ws(ws)
, _runtime(runtime)
{}

void ConversationEngine::sendPrompt(const String& prompt)
{
    Serial.println();
    Serial.print("Sending prompt: ");
    Serial.println(prompt);

    _tokenBuffer = "";

    _runtime.setListening();

    _ws.sendPrompt(prompt);
}

void ConversationEngine::onToken(const String& token)
{
    _tokenBuffer += token;
}

void ConversationEngine::onDone()
{
    Serial.print("Jannine: ");
    Serial.println(_tokenBuffer);

    Serial.println("[Stream Finished]");

    _tokenBuffer = "";
}