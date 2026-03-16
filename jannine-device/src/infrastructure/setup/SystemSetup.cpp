#include "SystemSetup.h"

SystemSetup::SystemSetup(
    Display& display,
    RobotRuntime& runtime,
    ConversationEngine& conversation,
    WebSocketClient& wsClient,
    NetworkSetup& network,
    SerialInputHandler& serial
)
: _display(display)
, _runtime(runtime)
, _conversation(conversation)
, _wsClient(wsClient)
, _network(network)
, _serial(serial)
{}

void SystemSetup::run()
{
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000);

    Serial.println("\n=== Jannine Device Started ===");

    _display.begin();
    _runtime.begin();

    _setupSerial();
    _setupWebSocketCallbacks();
    _setupNetwork();

    _conversation.sendPrompt("hello");

    Serial.println("=== Ready ===");
    Serial.print("You: ");
}

void SystemSetup::_setupSerial()
{
    _serial.setInputCallback(
        [this](const String& input)
        {
            _conversation.sendPrompt(input);
        }
    );
}

void SystemSetup::_setupWebSocketCallbacks()
{
    _wsClient.setDataCallback(
        [this](const uint8_t* data, size_t length)
        {
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
                Serial.print("You: ");
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