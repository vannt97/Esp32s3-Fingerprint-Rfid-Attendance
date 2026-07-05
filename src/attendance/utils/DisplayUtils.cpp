#include "DisplayUtils.h"

int rssiToBars(int32_t rssi)
{
    if (rssi >= -55) return 4;
    if (rssi >= -65) return 3;
    if (rssi >= -75) return 2;
    return 1;
}

String rssiToBarString(int32_t rssi)
{
    int bars = rssiToBars(rssi);
    String s = "";
    for (int i = 0; i < bars; i++) s += "|";
    return s;
}

String formatUptime(unsigned long ms)
{
    unsigned long s = ms / 1000;
    unsigned long m = s / 60;
    unsigned long h = m / 60;

    s %= 60;
    m %= 60;

    if (h > 0)
        return String(h) + "h" + (m < 10 ? "0" : "") + String(m) + "m";

    return String(m) + "m" + (s < 10 ? "0" : "") + String(s) + "s";
}

String formatBytes(uint32_t bytes)
{
    if (bytes >= 1024)
        return String(bytes / 1024) + "KB";
    return String(bytes) + "B";
}