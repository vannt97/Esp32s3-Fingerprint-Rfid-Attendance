#include "EnrollmentStore.h"

bool EnrollmentStore::begin()
{
    return _prefs.begin("enroll", false);
}

bool EnrollmentStore::saveFingerMapping(uint16_t employeeId, uint8_t fingerPosition, uint16_t templateId)
{
    char key[16];
    snprintf(key, sizeof(key), "f%u_%u", employeeId, fingerPosition);
    bool ok = _prefs.putUInt(key, templateId) > 0;

    char reverseKey[16];
    snprintf(reverseKey, sizeof(reverseKey), "t%u", templateId);
    _prefs.putUInt(reverseKey, employeeId);

    return ok;
}

bool EnrollmentStore::saveCardMapping(uint16_t employeeId, const String &uid)
{
    char key[16];
    snprintf(key, sizeof(key), "c%u", employeeId);
    return _prefs.putString(key, uid) > 0;
}

bool EnrollmentStore::getFingerMapping(uint16_t employeeId, uint8_t fingerPosition, uint16_t &templateIdOut)
{
    char key[16];
    snprintf(key, sizeof(key), "f%u_%u", employeeId, fingerPosition);
    if (!_prefs.isKey(key))
        return false;
    templateIdOut = (uint16_t)_prefs.getUInt(key);
    return true;
}

bool EnrollmentStore::getCardMapping(uint16_t employeeId, String &uidOut)
{
    char key[16];
    snprintf(key, sizeof(key), "c%u", employeeId);
    if (!_prefs.isKey(key))
        return false;
    uidOut = _prefs.getString(key);
    return true;
}

bool EnrollmentStore::findEmployeeByTemplateId(uint16_t templateId, uint16_t &employeeIdOut)
{
    char key[16];
    snprintf(key, sizeof(key), "t%u", templateId);
    if (!_prefs.isKey(key))
        return false;
    employeeIdOut = (uint16_t)_prefs.getUInt(key);
    return true;
}

bool EnrollmentStore::removeFingerMapping(uint16_t employeeId, uint8_t fingerPosition, uint16_t &deletedTemplateIdOut)
{
    if (!getFingerMapping(employeeId, fingerPosition, deletedTemplateIdOut))
        return false;

    char key[16];
    snprintf(key, sizeof(key), "f%u_%u", employeeId, fingerPosition);
    _prefs.remove(key);

    char reverseKey[16];
    snprintf(reverseKey, sizeof(reverseKey), "t%u", deletedTemplateIdOut);
    _prefs.remove(reverseKey);

    return true;
}

bool EnrollmentStore::removeCardMapping(uint16_t employeeId)
{
    char key[16];
    snprintf(key, sizeof(key), "c%u", employeeId);
    if (!_prefs.isKey(key))
        return false;
    _prefs.remove(key);
    return true;
}
