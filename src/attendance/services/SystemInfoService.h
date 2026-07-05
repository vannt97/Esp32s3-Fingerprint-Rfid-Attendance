#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <esp_system.h>
#include "services/WifiManager.h"  // dùng WifiStatus enum từ đây
#include "services/ChipInfo.h"

// Đổi tên để không conflict với WifiStatus enum trong WifiManager
struct WifiInfo {
    bool      connected;
    String    ssid;
    int32_t   rssi;
    String    ip;
};

struct SystemStatus {
    uint32_t      usedHeap;
    uint32_t      totalHeap;
    unsigned long uptimeMs;
};

struct SystemInfo {
    WifiInfo     wifi;    // đổi tên
    SystemStatus system;
};

class SystemInfoService {
public:
    SystemInfo   getInfo() const;
    WifiInfo     getWifiStatus() const;   // đổi return type
    SystemStatus getSystemStatus() const;
};