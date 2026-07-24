#pragma once
#include <Arduino.h>

// Ghi log chấm công (check-in) vào LittleFS, dạng CSV append-only
// "/attendance.log" (epoch,employeeId,method mỗi dòng). Chưa có
// xoá/xoay vòng/đồng bộ — đó là việc của ApiService khi nối server sau.
class AttendanceLog
{
public:
    bool begin();

    // method: 'F' (vân tay) hoặc 'C' (thẻ)
    bool append(uint16_t employeeId, char method, time_t epoch);

    // Debug tạm — in toàn bộ file ra Serial để verify sống sót qua reset.
    void dumpToSerial();

private:
    static constexpr const char *PATH = "/attendance.log";
};
