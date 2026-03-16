#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager();
    
    // Connection management
    bool connect(const char* ssid, const char* password);
    bool isConnected() const;
    String getLocalIP() const;
    
    // Status
    void printStatus() const;

private:
    const char* currentSSID;
    const char* currentPassword;
};

#endif // WIFI_MANAGER_H
