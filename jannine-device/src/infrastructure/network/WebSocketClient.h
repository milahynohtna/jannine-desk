#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <functional>

using namespace websockets;

class WebSocketClient {
public:
    using TokenCallback = std::function<void(const String&)>;
    using DoneCallback = std::function<void()>;
    using ConnectionCallback = std::function<void(bool)>;
    using DataCallback = std::function<void(const uint8_t*, size_t)>;

    WebSocketClient();
    ~WebSocketClient();

    bool connect(const char* serverUrl);
    void disconnect();
    bool isConnected() const;
    void poll();

    // ───────────── SEND ─────────────
    void sendText(const String& text);                   // 🔥 RAW TEXT (control)
    void sendBytes(const uint8_t* data, size_t len);     // binary audio

    // ───────────── CALLBACK ─────────────
    void setTokenCallback(TokenCallback callback);
    void setDoneCallback(DoneCallback callback);
    void setConnectionCallback(ConnectionCallback callback);
    void setDataCallback(DataCallback callback);

private:
    WebsocketsClient client;
    bool connected;

    TokenCallback onToken;
    DoneCallback onDone;
    ConnectionCallback onConnectionChange;
    DataCallback onData;

    void onMessageCallback(const WebsocketsMessage& message);
    void onEventsCallback(WebsocketsEvent event, const String& data);
    void handleJsonMessage(const String& payload);
};

#endif