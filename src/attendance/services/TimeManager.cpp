#include "TimeManager.h"

TimeManager::TimeManager(const char *timezone)
{
    // Việt Nam không đổi giờ mùa hè nên daylightOffset = 0
    _gmtOffset = GMT_OFFSET;
    _daylightOffset = DAYLIGHT_OFFSET;
}

void TimeManager::begin()
{
    // TZ tường minh (Việt Nam, không DST) thay vì dựa vào hành vi ngầm
    // của configTime() — configTzTime() tự set TZ đúng theo chuỗi POSIX.
    configTzTime("<+07>-7", _ntpServer, _ntpServer2);
}

bool TimeManager::isSynced()
{
    struct tm timeinfo;
    return getLocalTime(&timeinfo); // timeout = 0, không chờ
}

TimeData TimeManager::getTime()
{
    struct tm timeinfo;
    TimeData data = {0, 0, 0, 0, 0, 0, false};

    if (!getLocalTime(&timeinfo, 0))
    { // chờ tối đa 5 giây
        return data;
    }

    data.hour = timeinfo.tm_hour;
    data.minute = timeinfo.tm_min;
    data.second = timeinfo.tm_sec;
    data.day = timeinfo.tm_mday;
    data.month = timeinfo.tm_mon + 1;    // tm_mon bắt đầu từ 0
    data.year = timeinfo.tm_year + 1900; // tm_year tính từ 1900
    data.isValid = true;

    return data;
}

// String TimeManager::getTimeString()
// {
//     TimeData t = getTime();
//     if (!t.isValid)
//         return "--:--";

//     char buf[6];
//     snprintf(buf, sizeof(buf), "%02d:%02d", t.hour, t.minute);
//     return String(buf);
// }

// String TimeManager::getDateString()
// {
//     TimeData t = getTime();
//     if (!t.isValid)
//         return "--/--/----";

//     char buf[11];
//     snprintf(buf, sizeof(buf), "%02d/%02d/%04d", t.day, t.month, t.year);
//     return String(buf);
// }

TimeStringData TimeManager::getTimeAndDate()
{
    TimeData t = getTime(); // gọi getLocalTime() 1 lần duy nhất

    if (!t.isValid)
        return {"--:--", "--/--/----"};

    char timeBuf[6];
    char dateBuf[11];

    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d", t.hour, t.minute);
    snprintf(dateBuf, sizeof(dateBuf), "%02d/%02d/%04d", t.day, t.month, t.year);

    return {String(timeBuf), String(dateBuf)};
}

time_t TimeManager::getEpoch()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 0))
        return 0;
    return mktime(&timeinfo);
}