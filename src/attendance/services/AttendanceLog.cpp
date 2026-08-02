#include "AttendanceLog.h"
#include <LittleFS.h>

bool AttendanceLog::begin()
{
    if (!LittleFS.begin(true))
        return false;
    _truncateIncompleteTail();
    return true;
}

bool AttendanceLog::append(uint16_t employeeId, char method, time_t epoch)
{
    char line[32];
    int len = snprintf(line, sizeof(line), "%ld,%u,%c\n", (long)epoch, employeeId, method);
    if (len <= 0)
        return false;

    File f = LittleFS.open(PATH, "a");
    if (!f)
        return false;

    f.write((const uint8_t *)line, len);
    f.close();
    return true;
}

size_t AttendanceLog::size()
{
    File f = LittleFS.open(PATH, "r");
    if (!f)
        return 0;
    size_t sz = f.size();
    f.close();
    return sz;
}

String AttendanceLog::readRange(size_t fromOffset, size_t maxBytes)
{
    File f = LittleFS.open(PATH, "r");
    if (!f)
        return String();

    size_t sz = f.size();
    if (fromOffset >= sz)
    {
        f.close();
        return String();
    }

    f.seek(fromOffset);
    size_t toRead = min(maxBytes, sz - fromOffset);

    String out;
    out.reserve(toRead);
    for (size_t i = 0; i < toRead && f.available(); i++)
        out += (char)f.read();

    f.close();
    return out;
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

void AttendanceLog::_truncateIncompleteTail()
{
    File f = LittleFS.open(PATH, "r");
    if (!f)
        return;

    size_t sz = f.size();
    if (sz == 0)
    {
        f.close();
        return;
    }

    f.seek(sz - 1);
    bool endsWithNewline = (f.read() == '\n');
    if (endsWithNewline)
    {
        f.close();
        return;
    }

    // Dòng cuối dở dang (mất điện giữa lúc ghi) — đọc hết, cắt tới '\n'
    // hoàn chỉnh cuối cùng, ghi đè lại file.
    f.seek(0);
    String content;
    content.reserve(sz);
    while (f.available())
        content += (char)f.read();
    f.close();

    int lastNewline = content.lastIndexOf('\n');
    File out = LittleFS.open(PATH, "w");
    if (!out)
        return;
    if (lastNewline >= 0)
        out.write((const uint8_t *)content.c_str(), lastNewline + 1);
    out.close();
}