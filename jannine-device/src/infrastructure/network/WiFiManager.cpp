#include "WiFiManager.h"

WiFiManager::WiFiManager() 
    : currentSSID(nullptr), currentPassword(nullptr) {
}

bool WiFiManager::connect(const char* ssid, const char* password) {
    currentSSID = ssid;
    currentPassword = password;

    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int attempts = 0;
    const int maxAttempts = 30; // 15 seconds timeout

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        printStatus();
        return true;
    } else {
        Serial.println("\nWiFi Connection Failed!");
        return false;
    }
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getLocalIP() const {
    return WiFi.localIP().toString();
}

void WiFiManager::printStatus() const {
    Serial.println("WiFi Status:");
    Serial.print("  SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("  IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("  Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}
