#ifndef NETWORK_SETUP_H
#define NETWORK_SETUP_H

#include <Arduino.h>
#include "WiFiManager.h"
#include "WebSocketClient.h"

class NetworkSetup {
public:
    NetworkSetup(WiFiManager& wifi, WebSocketClient& ws);
    
    // Setup methods
    bool setupWiFi(const char* ssid, const char* password);
    bool setupWebSocket(const char* serverUrl);
    
    // Callback setters for WebSocket events
    typedef std::function<void(const String&)> TokenCallback;
    typedef std::function<void()> DoneCallback;
    typedef std::function<void(bool)> ConnectionCallback;
    
    void setTokenCallback(TokenCallback callback);
    void setDoneCallback(DoneCallback callback);
    void setConnectionCallback(ConnectionCallback callback);
    
    // Reconnection handling
    void checkAndReconnect(uint32_t reconnectDelayMs = 2000);
    
private:
    WiFiManager& wifiManager;
    WebSocketClient& wsClient;
    
    TokenCallback onToken;
    DoneCallback onDone;
    ConnectionCallback onConnection;
};

#endif // NETWORK_SETUP_H
