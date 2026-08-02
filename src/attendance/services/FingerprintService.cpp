#include "FingerprintService.h"
#include "pins.h"

FingerprintService::FingerprintService()
    : _fpSerial(1),
      _finger(&_fpSerial)
{
}

bool FingerprintService::begin()
{
    _fpSerial.begin(FP_BAUD, SERIAL_8N1, FP_RX_PIN, FP_TX_PIN);
    _finger.begin(FP_BAUD);
    delay(100);

    _connected = _finger.verifyPassword();
    if (_connected && _finger.getParameters() == FINGERPRINT_OK)
    {
        _capacity = _finger.capacity;
    }

    _prefs.begin("fp", false);
    return _connected;
}

FingerStepResult FingerprintService::captureStep()
{
    _lastError = _finger.getImage();
    if (_lastError == FINGERPRINT_NOFINGER)
        return FingerStepResult::NO_FINGER;
    if (_lastError == FINGERPRINT_OK)
        return FingerStepResult::CAPTURED;
    return FingerStepResult::ERROR;
}

bool FingerprintService::isFingerRemoved()
{
    return _finger.getImage() == FINGERPRINT_NOFINGER;
}

FingerVerifyResult FingerprintService::verifyStep(uint16_t &templateIdOut, uint16_t &confidenceOut)
{
    _lastError = _finger.getImage();
    if (_lastError == FINGERPRINT_NOFINGER)
        return FingerVerifyResult::NO_FINGER;
    if (_lastError != FINGERPRINT_OK)
        return FingerVerifyResult::ERROR;

    _lastError = _finger.image2Tz();
    if (_lastError != FINGERPRINT_OK)
        return FingerVerifyResult::ERROR;

    _lastError = _finger.fingerSearch();
    if (_lastError == FINGERPRINT_OK)
    {
        templateIdOut = _finger.fingerID;
        confidenceOut = _finger.confidence;
        return FingerVerifyResult::MATCHED;
    }
    if (_lastError == FINGERPRINT_NOTFOUND)
        return FingerVerifyResult::NOT_FOUND;
    return FingerVerifyResult::ERROR;
}

bool FingerprintService::convertImage(uint8_t slot)
{
    _lastError = _finger.image2Tz(slot);
    return _lastError == FINGERPRINT_OK;
}

bool FingerprintService::createModel()
{
    _lastError = _finger.createModel();
    return _lastError == FINGERPRINT_OK;
}

uint16_t FingerprintService::allocateNextTemplateId()
{
    uint32_t nextId = _prefs.getUInt("nextId", 1);
    if (nextId < 1 || nextId > _capacity)
        return 0;

    _prefs.putUInt("nextId", nextId + 1);
    return (uint16_t)nextId;
}

bool FingerprintService::storeModel(uint16_t id)
{
    _lastError = _finger.storeModel(id);
    return _lastError == FINGERPRINT_OK;
}

bool FingerprintService::deleteTemplate(uint16_t id)
{
    _lastError = _finger.deleteModel(id);
    return _lastError == FINGERPRINT_OK;
}

const char *FingerprintService::lastErrorString() const
{
    switch (_lastError)
    {
    case FINGERPRINT_OK:
        return "OK";
    case FINGERPRINT_NOTFOUND:
        return "Not found";
    case FINGERPRINT_PACKETRECIEVEERR:
        return "Sensor comm error";
    case FINGERPRINT_NOFINGER:
        return "No finger detected";
    case FINGERPRINT_IMAGEFAIL:
        return "Imaging failed";
    case FINGERPRINT_IMAGEMESS:
        return "Image too messy";
    case FINGERPRINT_FEATUREFAIL:
        return "Feature extract failed";
    case FINGERPRINT_NOMATCH:
        return "No match";
    case FINGERPRINT_ENROLLMISMATCH:
        return "Fingers did not match";
    default:
        return "Unknown error";
    }
}
