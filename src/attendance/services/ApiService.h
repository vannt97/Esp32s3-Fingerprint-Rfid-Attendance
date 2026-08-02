#pragma once
#include <Arduino.h>
#include <Preferences.h>

class EmployeeStore;
class AttendanceLog;

// Nói chuyện HTTP thường (không TLS) với server Django — theo đúng
// pattern đã có ở WeatherService (HTTPClient + ArduinoJson). Mọi hàm tự
// kiểm tra WiFi trước khi thử kết nối và dùng timeout ngắn, tránh đứng
// hình UI khi mất mạng/server sập.
class ApiService
{
public:
    bool begin();

    // GET /api/employees/ — thành công thì cập nhật EmployeeStore (RAM + cache).
    bool fetchEmployees(EmployeeStore &store);

    // POST /api/attendance/ 1 bản ghi. Trả true nếu HTTP 200/201.
    bool pushAttendanceRecord(uint16_t employeeId, char method, time_t epoch,
                               const String &clientRecordId);

    // Đọc phần AttendanceLog chưa đồng bộ (tối đa ~10 bản ghi/lần gọi),
    // POST từng dòng, chỉ lưu tiến độ (offset) sau khi gửi thành công.
    void syncPendingAttendance(AttendanceLog &log);

private:
    static constexpr int MAX_RECORDS_PER_CALL = 10;

    Preferences _prefs; // namespace "sync", key "offset"
};