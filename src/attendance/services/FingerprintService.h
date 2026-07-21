#pragma once
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <Preferences.h>

enum class FingerStepResult
{
    NO_FINGER,
    CAPTURED,
    ERROR
};

// Bọc Adafruit_Fingerprint (AS608, UART1) theo pin/baud trong pins.h.
// Mỗi hàm chỉ gọi 1 lệnh UART duy nhất (không dùng vòng while chờ) để phù
// hợp với vòng loop() cooperative của ScreenManager — screen tự gọi lại
// các hàm này mỗi tick thay cho việc chờ block bên trong service.
class FingerprintService
{
public:
    FingerprintService();

    bool begin();
    bool isConnected() const { return _connected; }

    FingerStepResult captureStep();
    bool isFingerRemoved();
    bool convertImage(uint8_t slot);
    bool createModel();
    uint16_t allocateNextTemplateId();
    bool storeModel(uint16_t id);
    const char *lastErrorString() const;

private:
    HardwareSerial _fpSerial;
    Adafruit_Fingerprint _finger;
    Preferences _prefs;
    bool _connected = false;
    uint16_t _capacity = 127;
    uint8_t _lastError = FINGERPRINT_OK;
};
