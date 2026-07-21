#include "EnrollScreen.h"
#include "display/ScreenManager.h"

EnrollScreen::EnrollScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void EnrollScreen::onEnter()
{
    _menuIdx = 0;
    _render();
}

void EnrollScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void EnrollScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);
    _displayManager.print(0, 0, "Enroll");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        int y = 16 + i * 12;
        if (i == _menuIdx)
        {
            _displayManager.fillRect(0, y - 1, SCREEN_WIDTH, 11, SSD1306_WHITE);
            _displayManager.setTextColor(SSD1306_BLACK);
        }
        else
        {
            _displayManager.setTextColor(SSD1306_WHITE);
        }
        _displayManager.print(4, y, ITEM_NAMES[i]);
    }

    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.print(89, 53, "Exit");
    _displayManager.update();
}

void EnrollScreen::loop()
{
    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        if (_menuIdx > 0) { _menuIdx--; _render(); }
        break;
    case Button::DOWN:
        if (_menuIdx < ITEM_COUNT - 1) { _menuIdx++; _render(); }
        break;
    case Button::SELECT:
        _screenManager.setPendingEnrollTarget(_menuIdx == 0 ? EnrollTarget::FINGER : EnrollTarget::CARD);
        _screenManager.showScreen(ScreenId::EMPLOYEE_SELECT);
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::EMPLOYEE);
        break;
    default:
        break;
    }
}
