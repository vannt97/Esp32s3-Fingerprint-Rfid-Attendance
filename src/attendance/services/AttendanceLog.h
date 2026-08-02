#pragma once
#include <Arduino.h>

// Ghi log chấm công (check-in) vào LittleFS, dạng CSV append-only
// "/attendance.log" (epoch,employeeId,method mỗi dòng). Chưa có
// xoá/xoay vòng — ApiService đọc dần qua size()/readRange() để đồng bộ,
// không xoá dòng nào khỏi file này.
class AttendanceLog
{
public:
    bool begin();

    // method: 'F' (vân tay) hoặc 'C' (thẻ)
    bool append(uint16_t employeeId, char method, time_t epoch);

    size_t size();

    // Đọc tối đa maxBytes byte kể từ fromOffset — dùng cho ApiService rút
    // dần phần log chưa đồng bộ, tránh đọc nguyên backlog lớn 1 lần.
    String readRange(size_t fromOffset, size_t maxBytes = 512);

    // Debug tạm — in toàn bộ file ra Serial để verify sống sót qua reset.
    void dumpToSerial();

private:
    static constexpr const char *PATH = "/attendance.log";

    // Cắt bỏ dòng cuối dở dang (mất điện giữa lúc ghi) nếu có, gọi 1 lần
    // trong begin() để ApiService không bao giờ phải parse dòng lỗi.
    void _truncateIncompleteTail();
};