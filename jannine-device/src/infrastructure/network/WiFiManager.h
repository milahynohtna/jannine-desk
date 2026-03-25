#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <config/WebSocketConfig.h>

class WiFiManager {
public:
    WiFiManager();
    
    // Connection management
    bool connect(const char* ssid, const char* password);
    static bool isConnected();
    // NOLINTNEXTLINE(readability-static-accessed-through-instance)
    static String getLocalIP();
    
    // Status
    static void printStatus();

private:
    const char* currentSSID;
    const char* currentPassword;

};

#endif // WIFI_MANAGER_H
