#pragma once
#include <Arduino.h>  // String type
#include <stdint.h>

// Chuyển RSSI (dBm) thành số bars (1-4)
int    rssiToBars(int32_t rssi);
String rssiToBarString(int32_t rssi);
String formatUptime(unsigned long ms);
String formatBytes(uint32_t bytes);