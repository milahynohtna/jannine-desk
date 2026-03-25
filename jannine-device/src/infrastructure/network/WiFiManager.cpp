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

    while (WiFiClass::status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFiClass::status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        printStatus();
        return true;
    }

    Serial.println("\nWiFi connection FAILED!");
    return false;
}

// NOLINTNEXTLINE(readability-static-accessed-through-instance)
bool WiFiManager::isConnected() {
    return WiFiClass::status() == WL_CONNECTED;
}

String WiFiManager::getLocalIP(){
    return WiFi.localIP().toString();
}

void WiFiManager::printStatus() {
    Serial.println("WiFi Status:");
    Serial.print("  SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("  IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("  Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}
