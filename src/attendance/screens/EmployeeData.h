#pragma once

// Danh sách nhân viên mock dùng chung cho UsersScreen (xem) và
// EmployeeSelectScreen (chọn trước khi enroll). Sẽ thay bằng dữ liệu
// đồng bộ từ server khi có ApiService — chỉ cần đổi nguồn dữ liệu ở đây,
// UI dùng EMPLOYEES/EMPLOYEE_COUNT không cần đổi.
struct Employee
{
    uint16_t id;
    const char *name;
};

static constexpr Employee EMPLOYEES[] = {
    {1, "Nguyen Van A"},
    {2, "Tran Thi B"},
    {3, "Le Van C"},
    {4, "Pham Thi D"},
    {5, "Hoang Van E"},
};
static constexpr int EMPLOYEE_COUNT = 5;
