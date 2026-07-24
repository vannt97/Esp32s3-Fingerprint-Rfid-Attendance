#pragma once
#include <Arduino.h>
#include <Preferences.h>

// Lưu tạm mapping (employee <-> vân tay/thẻ) vào NVS bằng Preferences.
// Bản tối giản chỉ ghi tuần tự, phục vụ bước gắn ID nhân viên; sẽ thay
// bằng LittleFS + đồng bộ server ở task sau (không đọc lại/tra cứu ở đây).
class EnrollmentStore
{
public:
    bool begin();

    bool saveFingerMapping(uint16_t employeeId, uint8_t fingerPosition, uint16_t templateId);
    bool saveCardMapping(uint16_t employeeId, const String &uid);

    // Đọc lại — dùng để verify sau khi ghi, và cho luồng check-in sau này.
    bool getFingerMapping(uint16_t employeeId, uint8_t fingerPosition, uint16_t &templateIdOut);
    bool getCardMapping(uint16_t employeeId, String &uidOut);

    // Tra cứu ngược cho check-in: AS608 fingerSearch() trả về templateId,
    // cần suy ra employeeId tương ứng.
    bool findEmployeeByTemplateId(uint16_t templateId, uint16_t &employeeIdOut);

private:
    Preferences _prefs;
};
