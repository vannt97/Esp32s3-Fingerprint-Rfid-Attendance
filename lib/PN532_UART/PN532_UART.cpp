#include "PN532_UART.h"

// PN532 frame constants
#define TFI_HOST2PN532  0xD4
#define TFI_PN5322HOST  0xD5

#define CMD_GETFIRMWAREVERSION  0x02
#define CMD_SAMCONFIGURATION    0x14
#define CMD_INLISTPASSIVETARGET 0x4A

static const uint8_t ACK[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};

// ---------- public ----------

PN532_UART::PN532_UART(HardwareSerial &serial) : _ser(serial) {}

void PN532_UART::begin() {
    wakeup();
}

uint32_t PN532_UART::getFirmwareVersion() {
    uint8_t cmd[] = {CMD_GETFIRMWAREVERSION};
    if (!sendFrame(cmd, 1)) return 0;

    uint8_t buf[8];
    uint8_t len = 0;
    if (!readFrame(buf, sizeof(buf), len, 500) || len < 5) return 0;

    // buf: [0x03, IC, VER, REV, SUP]
    return ((uint32_t)buf[1] << 24) | ((uint32_t)buf[2] << 16) |
           ((uint32_t)buf[3] << 8)  |  (uint32_t)buf[4];
}

bool PN532_UART::SAMConfig() {
    uint8_t cmd[] = {CMD_SAMCONFIGURATION, 0x01, 0x14, 0x01};
    if (!sendFrame(cmd, 4)) return false;
    uint8_t buf[4];
    uint8_t len = 0;
    return readFrame(buf, sizeof(buf), len, 500);
}

bool PN532_UART::readPassiveTargetID(uint8_t *uid, uint8_t *uidLen, uint16_t timeout) {
    drain();
    uint8_t cmd[] = {CMD_INLISTPASSIVETARGET, 0x01, 0x00};
    if (!sendFrame(cmd, 3)) return false;

    uint8_t buf[20];
    uint8_t len = 0;
    if (!readFrame(buf, sizeof(buf), len, timeout)) return false;

    // buf: [0x4B, NbTg, Tg, ATQA(2), SAK, NfcIdLen, NfcId...]
    if (len < 7 || buf[1] == 0) return false;

    *uidLen = buf[6];
    memcpy(uid, &buf[7], *uidLen);
    return true;
}

// ---------- private ----------

void PN532_UART::wakeup() {
    // HSU wakeup: 0x55 preamble + zeros to sync baud rate
    uint8_t seq[] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    _ser.write(seq, sizeof(seq));
    delay(50);
    drain();
}

void PN532_UART::drain() {
    uint32_t t = millis() + 20;
    while (millis() < t) {
        if (_ser.available()) { _ser.read(); t = millis() + 5; }
    }
}

bool PN532_UART::sendFrame(uint8_t *data, uint8_t dataLen) {
    // Frame: 00 00 FF [LEN] [LCS] [D4] [data...] [DCS] 00
    uint8_t len = dataLen + 1; // +1 for TFI
    uint8_t lcs = (uint8_t)(~len + 1);

    uint8_t dcs = TFI_HOST2PN532;
    for (uint8_t i = 0; i < dataLen; i++) dcs += data[i];
    dcs = (uint8_t)(~dcs + 1);

    _ser.write((uint8_t)0x00);
    _ser.write((uint8_t)0x00);
    _ser.write((uint8_t)0xFF);
    _ser.write(len);
    _ser.write(lcs);
    _ser.write((uint8_t)TFI_HOST2PN532);
    _ser.write(data, dataLen);
    _ser.write(dcs);
    _ser.write((uint8_t)0x00);

    return waitACK(1000);
}

bool PN532_UART::waitACK(uint16_t timeout) {
    uint32_t deadline = millis() + timeout;
    uint8_t buf[6];
    uint8_t i = 0;

    while (millis() < deadline) {
        if (_ser.available()) {
            buf[i++] = _ser.read();
            if (i == 6) return memcmp(buf, ACK, 6) == 0;
        }
    }
    return false;
}

bool PN532_UART::readFrame(uint8_t *buf, uint8_t bufSize, uint8_t &outLen, uint16_t timeout) {
    uint32_t deadline = millis() + timeout;

    // Find start code: 00 00 FF
    uint8_t seq = 0;
    while (millis() < deadline) {
        if (!_ser.available()) continue;
        uint8_t b = _ser.read();
        if      (seq == 0 && b == 0x00) seq = 1;
        else if (seq == 1 && b == 0x00) seq = 2;
        else if (seq >= 1 && b == 0xFF) { seq = 3; break; }
        else seq = (b == 0x00) ? 1 : 0;
    }
    if (seq != 3) return false;

    // LEN + LCS
    uint32_t t2 = millis() + 100;
    while (_ser.available() < 2 && millis() < t2) delay(1);
    if (_ser.available() < 2) return false;
    uint8_t len = _ser.read();
    uint8_t lcs = _ser.read();
    if ((uint8_t)(len + lcs) != 0x00) return false;

    // TFI + data + DCS + postamble
    uint8_t need = len + 2;
    uint32_t t3 = millis() + (uint32_t)(timeout > 200 ? 200 : timeout);
    while (_ser.available() < need && millis() < t3) delay(1);
    if (_ser.available() < need) return false;

    uint8_t tfi = _ser.read();
    if (tfi != TFI_PN5322HOST) {
        for (int i = 0; i < len + 1; i++) if (_ser.available()) _ser.read();
        return false;
    }

    uint8_t dataLen = len - 1;
    outLen = (dataLen < bufSize) ? dataLen : bufSize;
    uint8_t dcs = tfi;

    for (uint8_t i = 0; i < dataLen; i++) {
        uint8_t b = _ser.available() ? _ser.read() : 0;
        dcs += b;
        if (i < bufSize) buf[i] = b;
    }
    uint8_t rdcs = _ser.available() ? _ser.read() : 0;
    if (_ser.available()) _ser.read(); // postamble

    return (uint8_t)(dcs + rdcs) == 0x00;
}
