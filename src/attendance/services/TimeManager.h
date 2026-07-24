#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "config.h"

struct TimeData {
    int hour;
    int minute;
    int second;
    int day;
    int month;
    int year;
    bool isValid;  // đã sync được chưa
};

struct TimeStringData {
    String time;  // "HH:MM"
    String date;  // "DD/MM/YYYY"
};

class TimeManager {
public:
    TimeManager(const char* timezone = TIMEZONE);

    // Khởi động — gọi sau khi WiFi đã kết nối
    void begin();

    // Kiểm tra đã sync được chưa
    bool isSynced();

    // Lấy thời gian hiện tại
    TimeData getTime();

    TimeStringData getTimeAndDate();

    // Epoch Unix — dùng làm timestamp cho log chấm công. Trả 0 nếu chưa sync NTP.
    time_t getEpoch();

private:
    const char* _ntpServer   = NTP_SERVER;
    const char* _ntpServer2  = NTP_SERVER2;
    long        _gmtOffset   = GMT_OFFSET;
    int         _daylightOffset = DAYLIGHT_OFFSET;
};