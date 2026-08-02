#pragma once
#include "IScreen.h"
#include "FingerNames.h"

class ScreenManager;

// Xem + xóa vân tay/thẻ đã enroll cho nhân viên đã chọn ở
// EmployeeSelectScreen. Danh sách ĐỘNG (khác FingerSelectScreen — tĩnh 10
// dòng) — chỉ liệt kê đúng những gì nhân viên này thực sự đã enroll.
class DeleteEnrollmentScreen : public IScreen
{
public:
    explicit DeleteEnrollmentScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class Mode
    {
        LIST,
        CONFIRM,
        DONE,
    };

    struct Item
    {
        bool isCard;
        uint8_t fingerPosition; // hợp lệ nếu !isCard
        uint16_t templateId;    // hợp lệ nếu !isCard
    };

    void _buildList();
    void _render();
    void _renderConfirm();
    void _renderDone();
    void _drawScrollbar();
    void _confirmDelete();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    Mode _mode = Mode::LIST;

    static constexpr int MAX_ITEMS = FINGER_COUNT + 1;
    Item _items[MAX_ITEMS];
    int  _itemCount = 0;

    int _selectedIndex = 0;
    int _windowStart   = 0;

    static constexpr int VISIBLE_COUNT = 3;
    static constexpr int ITEM_Y0       = 16;
    static constexpr int ITEM_HEIGHT   = 11;
};