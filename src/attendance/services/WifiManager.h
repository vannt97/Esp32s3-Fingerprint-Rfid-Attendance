#pragma once
#include <Arduino.h>
#include <WiFi.h>

enum class WifiStatus {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    FAILED
};

class WifiManager {
public:
    WifiManager(const char* ssid, const char* password);

    // Gọi trong setup()
    void begin();

    // Gọi trong loop() — tự reconnect nếu mất kết nối
    void loop();

    void connecting();

    bool        isConnected();
    bool        isConnecting();
    WifiStatus  getStatus();
    String      getIP();
    int         getRSSI();  // cường độ tín hiệu (dBm)

private:
    const char* _ssid;
    const char* _password;

    WifiStatus    _status           = WifiStatus::DISCONNECTED;
    unsigned long _lastReconnect    = 0;
    unsigned long _connectStartTime = 0;

    const unsigned long RECONNECT_INTERVAL = 10000;  // thử lại mỗi 10 giây
    const unsigned long CONNECT_TIMEOUT    = 15000;  // timeout 15 giây

    void _connect();
};