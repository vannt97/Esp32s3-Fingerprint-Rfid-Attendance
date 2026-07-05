#include "SystemInfoService.h"

WifiInfo SystemInfoService::getWifiStatus() const
{
    if (!WiFi.isConnected())
        return {false, "", 0, ""};

    return {
        .connected = true,
        .ssid      = WiFi.SSID(),
        .rssi      = WiFi.RSSI(),
        .ip        = WiFi.localIP().toString()
    };
}

SystemStatus SystemInfoService::getSystemStatus() const
{
    uint32_t total = ESP.getHeapSize();
    uint32_t free  = esp_get_free_heap_size();

    return {
        .usedHeap  = total - free,
        .totalHeap = total,
        .uptimeMs  = millis()
    };
}

SystemInfo SystemInfoService::getInfo() const
{
    return {
        .wifi   = getWifiStatus(),
        .system = getSystemStatus(),
        // .chip   = ChipInfo::get()
    };
}