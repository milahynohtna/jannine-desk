#ifndef NETWORK_SETUP_H
#define NETWORK_SETUP_H

#include <Arduino.h>
#include "WiFiManager.h"
#include "WebSocketClient.h"

class NetworkSetup {
public:

    NetworkSetup(WiFiManager& wifi, WebSocketClient& ws);
    
    // Setup methods
    bool setupWiFi(const char* ssid, const char* password) const;
    bool setupWebSocket(const char* serverUrl) const;
    
    // Callback setters for WebSocket events
    typedef std::function<void(const String&)> TokenCallback;
    typedef std::function<void()> DoneCallback;
    typedef std::function<void(bool)> ConnectionCallback;
    
    void setTokenCallback(const TokenCallback& callback);
    void setDoneCallback(const DoneCallback& callback);
    void setConnectionCallback(const ConnectionCallback& callback);
    
    // Reconnection handling
    void checkAndReconnect(uint32_t reconnectDelayMs = 2000) const;
    
private:
    WiFiManager& wifiManager;
    WebSocketClient& wsClient;
    
    TokenCallback onToken;
    DoneCallback onDone;
    ConnectionCallback onConnection;
};

#endif // NETWORK_SETUP_H
