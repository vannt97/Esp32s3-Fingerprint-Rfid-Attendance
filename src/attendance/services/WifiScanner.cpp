#include "WifiScanner.h"

WifiScanner::WifiScanner() {}

int WifiScanner::scan() {
    Serial.println("Dang quet WiFi...");

    // mode WIFI_STA cần thiết để scan
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int found = WiFi.scanNetworks();  // blocking scan

    if (found <= 0) {
        Serial.println("Khong tim thay mang nao!");
        _count = 0;
        return 0;
    }

    // giới hạn MAX_NETWORKS
    _count = min(found, MAX_NETWORKS);

    for (int i = 0; i < _count; i++) {
        _networks[i] = {
            .ssid       = WiFi.SSID(i),
            .rssi       = WiFi.RSSI(i),
            .encryption = (uint8_t)WiFi.encryptionType(i),
            .isOpen     = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
        };
    }

    // Xóa kết quả scan để giải phóng RAM
    WiFi.scanDelete();

    return _count;
}

int WifiScanner::getCount() const {
    return _count;
}

WifiNetwork WifiScanner::getNetwork(int index) const {
    if (index < 0 || index >= _count) {
        return {"", 0, 0, false};  // trả về rỗng nếu index sai
    }
    return _networks[index];
}