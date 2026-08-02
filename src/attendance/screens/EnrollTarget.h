#pragma once

// EnrollScreen/EmployeeScreen đặt giá trị này trước khi vào
// EmployeeSelectScreen, để biết sau khi chọn nhân viên xong thì rẽ sang
// FINGER_SELECT, CARD_SCAN hay DELETE_ENROLLMENT.
enum class EnrollTarget
{
    FINGER,
    CARD,
    DELETE,
};
