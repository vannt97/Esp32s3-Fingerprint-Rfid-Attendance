#include "EmployeeScreen.h"
#include "display/ScreenManager.h"

EmployeeScreen::EmployeeScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void EmployeeScreen::onEnter()
{
    _menuIdx = 0;
    _render();
}

void EmployeeScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void EmployeeScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);
    _displayManager.print(0, 0, "Employee");
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

void EmployeeScreen::loop()
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
        // TODO: navigate to Enroll/Users screen
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    default:
        break;
    }
}
