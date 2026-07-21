#pragma once
#include "IScreen.h"

class ScreenManager;

static constexpr const char *USER_NAMES[] = {
    "Nguyen Van A", "Tran Thi B", "Le Van C", "Pham Thi D", "Hoang Van E",
};
static constexpr int USER_COUNT = 5;

// Mô phỏng UI danh sách user, dữ liệu mock cứng (chưa có storage thật).
// Chỉ xem danh sách, không có trang chi tiết.
class UsersScreen : public IScreen
{
public:
    explicit UsersScreen(ScreenManager &sm);
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
