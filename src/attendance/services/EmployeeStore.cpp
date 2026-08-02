#include "EmployeeStore.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "screens/EmployeeData.h"

bool EmployeeStore::begin()
{
    File f = LittleFS.open(CACHE_PATH, "r");
    if (f)
    {
        bool ok = _parseAndApply(f, false);
        f.close();
        if (ok)
            return true;
    }

    _loadFallbackSeed();
    return true;
}

bool EmployeeStore::replaceAllFromStream(Stream &jsonStream)
{
    return _parseAndApply(jsonStream, true);
}

bool EmployeeStore::_parseAndApply(Stream &jsonStream, bool writeCache)
{
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonStream);
    if (err)
        return false;

    JsonArray arr = doc.as<JsonArray>();
    if (arr.isNull() || arr.size() == 0)
        return false;

    int n = 0;
    for (JsonObject obj : arr)
    {
        if (n >= MAX_EMPLOYEES)
            break;
        _employees[n].id = obj["id"].as<uint16_t>();
        strlcpy(_employees[n].name, obj["name"] | "", sizeof(_employees[n].name));
        n++;
    }
    if (n == 0)
        return false;
    _count = n;

    if (writeCache)
    {
        File f = LittleFS.open(CACHE_PATH, "w");
        if (f)
        {
            serializeJson(doc, f);
            f.close();
        }
    }
    return true;
}

void EmployeeStore::_loadFallbackSeed()
{
    _count = 0;
    for (int i = 0; i < EMPLOYEE_SEED_COUNT && i < MAX_EMPLOYEES; i++)
    {
        _employees[i].id = EMPLOYEE_SEED[i].id;
        strlcpy(_employees[i].name, EMPLOYEE_SEED[i].name, sizeof(_employees[i].name));
        _count++;
    }
}

bool EmployeeStore::findById(uint16_t id, Employee &out) const
{
    for (int i = 0; i < _count; i++)
    {
        if (_employees[i].id == id)
        {
            out = _employees[i];
            return true;
        }
    }
    return false;
}