#include "RfidService.h"
#include "pins.h"

RfidService::RfidService()
    : _nfc(-1, -1)
{
}

bool RfidService::begin()
{
    Wire.begin(PN532_SDA_PIN, PN532_SCL_PIN);
    Wire.setClock(100000);
    delay(50);

    _nfc.begin();
    uint32_t version = _nfc.getFirmwareVersion();
    _connected = version != 0;

    if (_connected)
        _nfc.SAMConfig();

    return _connected;
}

bool RfidService::pollCard(String &uidOut)
{
    uint8_t uid[7] = {0};
    uint8_t uidLen = 0;

    if (!_nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, POLL_TIMEOUT_MS))
        return false;

    char buf[3];
    uidOut = "";
    for (uint8_t i = 0; i < uidLen; i++)
    {
        snprintf(buf, sizeof(buf), "%02X", uid[i]);
        uidOut += buf;
        if (i < uidLen - 1)
            uidOut += ":";
    }
    return true;
}
