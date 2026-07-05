#pragma once

// ── Home Screen ───────────────────────────────
struct HomeData {
    String time;            // "20:10"
    String date;            // "08/03/2026"
    int         batteryPercent;  // 0 - 100
    bool        wifiConnected;   // true / false
};

struct MenuData {
    int currentScrollPos;
};