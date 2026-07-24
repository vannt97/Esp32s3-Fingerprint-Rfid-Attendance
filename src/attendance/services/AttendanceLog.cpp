#include "AttendanceLog.h"
#include <LittleFS.h>

bool AttendanceLog::begin()
{
    return LittleFS.begin(true);
}

bool AttendanceLog::append(uint16_t employeeId, char method, time_t epoch)
{
    File f = LittleFS.open(PATH, "a");
    if (!f)
        return false;

    f.printf("%ld,%u,%c\n", (long)epoch, employeeId, method);
    f.close();
    return true;
}

void AttendanceLog::dumpToSerial()
{
    File f = LittleFS.open(PATH, "r");
    if (!f)
    {
        Serial.println("  (empty)");
        return;
    }

    while (f.available())
        Serial.write(f.read());
    f.close();
}
