#include "ApiService.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "EmployeeStore.h"
#include "AttendanceLog.h"

bool ApiService::begin()
{
    return _prefs.begin("sync", false);
}

bool ApiService::fetchEmployees(EmployeeStore &store)
{
    if (WiFi.status() != WL_CONNECTED)
        return false;

    HTTPClient http;
    http.setConnectTimeout(2000);
    http.setTimeout(3000);
    http.begin(String(API_BASE_URL) + "/api/employees/");
    http.addHeader("Authorization", String("Token ") + API_DEVICE_TOKEN);

    int code = http.GET();
    if (code != HTTP_CODE_OK)
    {
        http.end();
        return false;
    }

    bool ok = store.replaceAllFromStream(http.getStream());
    http.end();
    return ok;
}

bool ApiService::pushAttendanceRecord(uint16_t employeeId, char method, time_t epoch,
                                      const String &clientRecordId)
{
    if (WiFi.status() != WL_CONNECTED)
        return false;

    struct tm tmUtc;
    gmtime_r(&epoch, &tmUtc);
    char isoBuf[25];
    snprintf(isoBuf, sizeof(isoBuf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
             tmUtc.tm_year + 1900, tmUtc.tm_mon + 1, tmUtc.tm_mday,
             tmUtc.tm_hour, tmUtc.tm_min, tmUtc.tm_sec);

    JsonDocument doc;
    doc["employee"] = employeeId;
    doc["method"] = String(method);
    doc["event_time"] = isoBuf;
    doc["client_record_id"] = clientRecordId;
    String body;
    serializeJson(doc, body);

    HTTPClient http;
    http.setConnectTimeout(2000);
    http.setTimeout(2500);
    http.begin(String(API_BASE_URL) + "/api/attendance/");
    http.addHeader("Authorization", String("Token ") + API_DEVICE_TOKEN);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(body);
    http.end();
    return code == 200 || code == 201;
}

bool ApiService::_parseLogLine(const String &line, time_t &epochOut, uint16_t &employeeIdOut, char &methodOut)
{
    long epochLong = 0;
    unsigned int employeeId = 0;
    char method = 0;
    if (sscanf(line.c_str(), "%ld,%u,%c", &epochLong, &employeeId, &method) != 3)
        return false;

    epochOut = (time_t)epochLong;
    employeeIdOut = (uint16_t)employeeId;
    methodOut = method;
    return true;
}

void ApiService::syncPendingAttendance(AttendanceLog &log)
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    size_t total = log.size();
    size_t offset = _prefs.getULong("offset", 0);
    if (offset > total)
        offset = total;

    int sent = 0;
    while (sent < MAX_RECORDS_PER_CALL && offset < total)
    {
        String chunk = log.readRange(offset, 512);
        int nl = chunk.indexOf('\n');
        if (nl < 0)
            break;

        String line = chunk.substring(0, nl);

        time_t epoch;
        uint16_t employeeId;
        char method;
        if (!_parseLogLine(line, epoch, employeeId, method))
        {

            offset += nl + 1;
            _prefs.putULong("offset", offset);
            continue;
        }

        String clientId = String((long)epoch) + "-" + String(employeeId) + "-" + String(method);
        if (!pushAttendanceRecord(employeeId, method, epoch, clientId))
            break;

        offset += nl + 1;
        _prefs.putULong("offset", offset);
        sent++;
    }
}