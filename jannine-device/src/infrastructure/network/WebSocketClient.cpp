#include "WebSocketClient.h"
#include <ArduinoJson.h>

#include <utility>

WebSocketClient::WebSocketClient()
    : connected(false),
      onToken(nullptr),
      onDone(nullptr),
      onConnectionChange(nullptr),
      onData(nullptr)
{
}

WebSocketClient::~WebSocketClient() {
    disconnect();
}

bool WebSocketClient::connect(const char* serverUrl) {
    Serial.println("Connecting to WebSocket...");

    // Re-register callbacks setiap kali connect dipanggil
    // (penting saat reconnect — object internal bisa reset)
    client.onMessage([this](const WebsocketsMessage& message) {
        this->onMessageCallback(message);
    });

    client.onEvent([this](WebsocketsEvent event, const String& data) {
        this->onEventsCallback(event, data);
    });

    bool result = client.connect(serverUrl);

    if (!result) {
        Serial.println("WebSocket connect failed!");
    }

    return result;
}

void WebSocketClient::disconnect() {
    // Selalu panggil close() — bahkan jika connected == false
    // Ini memastikan TCP socket dan state internal library bersih
    // sebelum connect() berikutnya dipanggil
    client.close();
    connected = false;
}

bool WebSocketClient::isConnected() const {
    return connected;
}

void WebSocketClient::poll() {
    client.poll();
}


void WebSocketClient::sendBytes(const uint8_t* data, size_t len)
{
    if (!connected) {
        Serial.println("Error: WebSocket not connected");
        return;
    }

    if (data == nullptr || len == 0) {
        return;
    }

    // Kirim sebagai binary frame
    client.sendBinary((const char*)data, len);
}

void WebSocketClient::sendText(const String& text)
{
    if (!connected) {
        Serial.println("Error: WebSocket not connected");
        return;
    }

    client.send(text);
}

void WebSocketClient::setTokenCallback(TokenCallback callback) {
    onToken = std::move(callback);
}

void WebSocketClient::setDoneCallback(DoneCallback callback) {
    onDone = std::move(callback);
}

void WebSocketClient::setConnectionCallback(ConnectionCallback callback) {
    onConnectionChange = std::move(callback);
}

void WebSocketClient::setDataCallback(DataCallback callback) {
    onData = std::move(callback);
}

void WebSocketClient::onMessageCallback(const WebsocketsMessage& message)
{
    if (message.isBinary()) {
        if (onData) {
            const auto* data =
                reinterpret_cast<const uint8_t*>(message.c_str());
            size_t len = message.length();
            if (len > 0) {
                onData(data, len);
            }
        }
        return;
    }

    String payload = message.data();
    handleJsonMessage(payload);
}

void WebSocketClient::onEventsCallback(WebsocketsEvent event, const String& data)
{
    if (event == WebsocketsEvent::ConnectionOpened) {
        connected = true;
        Serial.println("WebSocket Connected!");
        if (onConnectionChange) onConnectionChange(true);
    }
    else if (event == WebsocketsEvent::ConnectionClosed) {
        connected = false;
        Serial.println("WebSocket Disconnected!");
        if (onConnectionChange) onConnectionChange(false);
    }
}

void WebSocketClient::handleJsonMessage(const String& payload)
{
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (err) {
        Serial.print("JSON parse error: ");
        Serial.println(err.c_str());
        return;
    }

    String type = doc["type"] | "";

    if (type == "token") {
        String token = doc["data"] | "";
        if (onToken) onToken(token);
    }
    else if (type == "done") {
        if (onDone) onDone();
    }
}
