#include "WifiManager.h"

WifiManager::WifiManager(const char *ssid, const char *password)
    : _ssid(ssid), _password(password) {}

void WifiManager::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true); // ESP32 tự reconnect ở background
    _connect();
}

void WifiManager::_connect()
{
    _status = WifiStatus::CONNECTING;
    _connectStartTime = millis();
    WiFi.begin(_ssid, _password);
}

void WifiManager::loop()
{
    unsigned long now = millis();

    switch (_status)
    {
    case WifiStatus::CONNECTING:
        if (WiFi.status() == WL_CONNECTED)
        {
            _status = WifiStatus::CONNECTED;
            // Serial.printf("[WiFi] Da ket noi! IP: %s\n", getIP().c_str());
        }
        // Quá timeout → thử lại
        else if (now - _connectStartTime >= CONNECT_TIMEOUT)
        {
            // Serial.println("[WiFi] Ket noi that bai, thu lai...");
            _status = WifiStatus::FAILED;
            _lastReconnect = now;
        }
        break;

    case WifiStatus::CONNECTED:
        // Mất kết nối → chuyển sang DISCONNECTED
        if (WiFi.status() != WL_CONNECTED)
        {
            // Serial.println("[WiFi] Mat ket noi!");
            _status = WifiStatus::DISCONNECTED;
        }
        break;

    case WifiStatus::DISCONNECTED:
    case WifiStatus::FAILED:
        // Thử reconnect sau mỗi 10 giây
        if (now - _lastReconnect >= RECONNECT_INTERVAL)
        {
            _lastReconnect = now;
            _connect();
        }
        break;
    }
}

void WifiManager::connecting()
{
    switch (_status)
    {
    case WifiStatus::CONNECTING:
        if (WiFi.status() == WL_CONNECTED)
        {
            _status = WifiStatus::CONNECTED;
        }
        break;

    case WifiStatus::CONNECTED:
        // Mất kết nối → chuyển sang DISCONNECTED
        if (WiFi.status() != WL_CONNECTED)
        {
            // Serial.println("[WiFi] Mat ket noi!");
            _status = WifiStatus::DISCONNECTED;
        }
        break;

    case WifiStatus::DISCONNECTED:
    case WifiStatus::FAILED:
        break;
    }
}

bool WifiManager::isConnected()
{
    return _status == WifiStatus::CONNECTED;
}

bool WifiManager::isConnecting()
{
    return _status == WifiStatus::CONNECTING;
}

WifiStatus WifiManager::getStatus()
{
    return _status;
}

String WifiManager::getIP()
{
    if (!isConnected())
        return "0.0.0.0";
    return WiFi.localIP().toString();
}

int WifiManager::getRSSI()
{
    if (!isConnected())
        return 0;
    return WiFi.RSSI();
}