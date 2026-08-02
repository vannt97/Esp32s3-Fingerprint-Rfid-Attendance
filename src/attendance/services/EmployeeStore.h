#pragma once
#include <Arduino.h>

struct Employee
{
    uint16_t id;
    char name[32];
};

// Danh sách nhân viên trên thiết bị — cache LittleFS "/employees.json"
// đồng bộ từ Django qua ApiService::fetchEmployees(). Nếu chưa từng đồng
// bộ được lần nào (mất mạng ngay từ đầu) thì seed từ EMPLOYEE_SEED[]
// (screens/EmployeeData.h) để thiết bị vẫn dùng tạm được.
class EmployeeStore
{
public:
    bool begin();

    // Gọi bởi ApiService sau khi GET /api/employees/ thành công. Chỉ ghi
    // đè RAM + cache khi parse JSON thành công và mảng không rỗng.
    bool replaceAllFromStream(Stream &jsonStream);

    int count() const { return _count; }
    const Employee &at(int index) const { return _employees[index]; }
    bool findById(uint16_t id, Employee &out) const;

private:
    static constexpr int MAX_EMPLOYEES = 50;
    static constexpr const char *CACHE_PATH = "/employees.json";

    Employee _employees[MAX_EMPLOYEES];
    int _count = 0;

    bool _parseAndApply(Stream &jsonStream, bool writeCache);
    void _loadFallbackSeed();
};