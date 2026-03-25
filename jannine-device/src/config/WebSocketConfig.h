#ifndef WEBSOCKET_CONFIG_H
#define WEBSOCKET_CONFIG_H

// WiFi Configuration
#define WIFI_SSID "cctv"
#define WIFI_PASSWORD "cctvsoi56789"

// WebSocket Server Configuration
#define WS_SERVER_URL "ws://192.168.100.83:8765"

// JSON Buffer Sizes
#define JSON_DOC_SIZE 512

// Connection Settings
#define RECONNECT_DELAY_MS 2000
#define SERIAL_BAUD_RATE 115200
#define MAX_ATTEMPTS 30

#endif // WEBSOCKET_CONFIG_H
