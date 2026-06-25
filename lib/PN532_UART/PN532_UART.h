#pragma once
#include <Arduino.h>

class PN532_UART {
public:
    explicit PN532_UART(HardwareSerial &serial);

    void    begin();
    uint32_t getFirmwareVersion();
    bool    SAMConfig();
    bool    readPassiveTargetID(uint8_t *uid, uint8_t *uidLen, uint16_t timeout = 5000);

private:
    HardwareSerial &_ser;

    void    wakeup();
    bool    sendFrame(uint8_t *data, uint8_t dataLen);
    bool    waitACK(uint16_t timeout = 1000);
    bool    readFrame(uint8_t *buf, uint8_t bufSize, uint8_t &outLen, uint16_t timeout = 1000);
    void    drain();
};
