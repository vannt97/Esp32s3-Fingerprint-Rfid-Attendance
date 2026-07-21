#pragma once

// EnrollScreen chọn loại enroll (vân tay/thẻ) trước, EmployeeSelectScreen
// dùng giá trị này để biết sau khi chọn nhân viên xong thì rẽ sang
// FINGER_SELECT hay CARD_SCAN.
enum class EnrollTarget
{
    FINGER,
    CARD,
};
