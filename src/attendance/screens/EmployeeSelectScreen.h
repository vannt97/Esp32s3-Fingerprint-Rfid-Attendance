#pragma once
#include "IScreen.h"

class ScreenManager;

// Chọn nhân viên trước khi enroll vân tay/thẻ. EnrollScreen đặt
// EnrollTarget (FINGER/CARD) trước khi vào màn này; sau khi chọn xong,
// lưu employee id vào ScreenManager rồi rẽ sang FINGER_SELECT hoặc
// CARD_SCAN tương ứng.
class EmployeeSelectScreen : public IScreen
{
public:
    explicit EmployeeSelectScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    void _render();
    void _drawScrollbar();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    int _selectedIndex = 0;
    int _windowStart   = 0;

    static constexpr int VISIBLE_COUNT = 3;
    static constexpr int ITEM_Y0       = 16;
    static constexpr int ITEM_HEIGHT   = 11;
};
