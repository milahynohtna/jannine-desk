#include "NetworkSetup.h"
#include "config/WebSocketConfig.h"

NetworkSetup::NetworkSetup(WiFiManager& wifi, WebSocketClient& ws) 
    : wifiManager(wifi), wsClient(ws), onToken(nullptr), onDone(nullptr), onConnection(nullptr) {
}

bool NetworkSetup::setupWiFi(const char* ssid, const char* password) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    if (!wifiManager.connect(ssid, password)) {
        Serial.println("Failed to connect to WiFi!");
        return false;
    }
    
    Serial.println("WiFi connected successfully!");
    return true;
}

bool NetworkSetup::setupWebSocket(const char* serverUrl) {
    Serial.println("Connecting to WebSocket server...");
    
    if (onToken)      wsClient.setTokenCallback(onToken);
    if (onDone)       wsClient.setDoneCallback(onDone);
    if (onConnection) wsClient.setConnectionCallback(onConnection);
    
    if (!wsClient.connect(serverUrl)) {
        Serial.println("Failed to connect to WebSocket server!");
        return false;
    }
    
    Serial.println("WebSocket connected successfully!");
    return true;
}

void NetworkSetup::setTokenCallback(TokenCallback callback) {
    onToken = callback;
}

void NetworkSetup::setDoneCallback(DoneCallback callback) {
    onDone = callback;
}

void NetworkSetup::setConnectionCallback(ConnectionCallback callback) {
    onConnection = callback;
}

void NetworkSetup::checkAndReconnect(uint32_t reconnectDelayMs) {
    if (wsClient.isConnected()) return;

    Serial.println("WebSocket disconnected. Reconnecting...");

    // ── 1. Tutup koneksi lama secara eksplisit ────────────────────────────
    // Tanpa ini, ArduinoWebsockets mengirim request baru dengan state TCP
    // yang belum bersih → server terima data korup → 1002 protocol error
    wsClient.disconnect();

    // ── 2. Tunggu cukup lama agar server & TCP stack reset ────────────────
    // reconnectDelayMs dari Config.h (disarankan minimal 2000ms)
    delay(reconnectDelayMs);

    // ── 3. Re-register semua callbacks SEBELUM connect ────────────────────
    // connect() ulang membuat object internal baru, callbacks harus di-set ulang
    if (onToken)      wsClient.setTokenCallback(onToken);
    if (onDone)       wsClient.setDoneCallback(onDone);
    if (onConnection) wsClient.setConnectionCallback(onConnection);

    // ── 4. Connect ulang ──────────────────────────────────────────────────
    if (!wsClient.connect(WS_SERVER_URL)) {
        Serial.println("Reconnection failed!");
    } else {
        Serial.println("Reconnected successfully!");
    }
}
