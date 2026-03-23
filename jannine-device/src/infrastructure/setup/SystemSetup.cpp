#include "SystemSetup.h"

SystemSetup::SystemSetup(
    Display& display,
    RobotRuntime& runtime,
    ConversationEngine& conversation,
    WebSocketClient& wsClient,
    NetworkSetup& network)
: _display(display)
, _runtime(runtime)
, _conversation(conversation)
, _wsClient(wsClient)
, _network(network)
{}

void SystemSetup::run()
{
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000);

    Serial.printf("[MEM] Free heap     : %d bytes\n", ESP.getFreeHeap());
    Serial.printf("[MEM] Free PSRAM    : %d bytes\n", ESP.getFreePsram());
    Serial.printf("[MEM] Total PSRAM   : %d bytes\n", ESP.getPsramSize());

    Serial.println("\n=== Jannine Device Started ===");

    _display.begin();
    _runtime.begin();

    _setupWebSocketCallbacks();
    _setupNetwork();

    // 🔥 MULAI DARI LISTENING (INI KUNCI)
    _conversation.startListening();

    Serial.println("=== Voice Mode Ready ===");
}

void SystemSetup::_setupWebSocketCallbacks()
{
    _wsClient.setDataCallback(
        [this](const uint8_t* data, size_t length)
        {
            // startSpeaking() sudah punya _speakingTriggered sebagai guard
            // tidak perlu cek isPlaying() di sini
            _conversation.startSpeaking();
            _runtime.audio().write(data, length);
        }
    );

    _network.setTokenCallback(
        [this](const String& token)
        {
            _conversation.onToken(token);
        }
    );

    _network.setDoneCallback(
        [this]()
        {
            _conversation.onDone();
        }
    );

    _network.setConnectionCallback(
        [this](bool connected)
        {
            if (connected)
            {
                Serial.println("WebSocket Connected.");
                _runtime.setIdle();
            }
            else
            {
                Serial.println("WebSocket Disconnected.");
            }
        }
    );
}
void SystemSetup::_setupNetwork()
{
    if (!_network.setupWiFi(WIFI_SSID, WIFI_PASSWORD))
    {
        Serial.println("WiFi setup failed. Restarting...");
        delay(3000);
        ESP.restart();
    }

    if (!_network.setupWebSocket(WS_SERVER_URL))
    {
        Serial.println("WebSocket setup failed.");
    }
}