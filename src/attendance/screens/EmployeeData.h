#pragma once

// Dữ liệu "mồi" (fallback) — CHỈ dùng khi thiết bị chưa từng đồng bộ
// được với Django (chưa có cache "/employees.json" trong LittleFS).
// EmployeeStore::begin() là nơi duy nhất còn đọc file này; sau khi đồng
// bộ thành công lần đầu, dữ liệu ở đây không còn được dùng tới nữa.
struct EmployeeSeed
{
    uint16_t id;
    const char *name;
};

static constexpr EmployeeSeed EMPLOYEE_SEED[] = {
    {1, "Nguyen Van A"},
    {2, "Tran Thi B"},
    {3, "Le Van C"},
    {4, "Pham Thi D"},
    {5, "Hoang Van E"},
};
static constexpr int EMPLOYEE_SEED_COUNT = 5;