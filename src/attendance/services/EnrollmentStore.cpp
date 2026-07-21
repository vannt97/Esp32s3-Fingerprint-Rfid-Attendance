#include "EnrollmentStore.h"

bool EnrollmentStore::begin()
{
    return _prefs.begin("enroll", false);
}

bool EnrollmentStore::saveFingerMapping(uint16_t employeeId, uint8_t fingerPosition, uint16_t templateId)
{
    char key[16];
    snprintf(key, sizeof(key), "f%u_%u", employeeId, fingerPosition);
    return _prefs.putUInt(key, templateId) > 0;
}

bool EnrollmentStore::saveCardMapping(uint16_t employeeId, const String &uid)
{
    char key[16];
    snprintf(key, sizeof(key), "c%u", employeeId);
    return _prefs.putString(key, uid) > 0;
}
