#pragma once
#include <Arduino.h>
#include <WiFi.h>

struct WifiNetwork {
    String ssid;
    int32_t rssi;        // cường độ tín hiệu (dBm)
    uint8_t encryption;  // kiểu mã hóa
    bool isOpen;         // không cần mật khẩu
};

class WifiScanner {
public:
    WifiScanner();

    // Quét wifi — trả về số lượng mạng tìm thấy
    int scan();

    // Lấy danh sách sau khi scan
    int getCount() const;
    WifiNetwork getNetwork(int index) const;

    // In ra Serial cho debug
    // void printAll() const;

private:
    int _count = 0;
    static const int MAX_NETWORKS = 20;
    WifiNetwork _networks[MAX_NETWORKS];
};