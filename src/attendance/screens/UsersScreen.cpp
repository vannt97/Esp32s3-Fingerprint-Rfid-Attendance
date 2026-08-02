#include "UsersScreen.h"
#include "display/ScreenManager.h"

UsersScreen::UsersScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void UsersScreen::onEnter()
{
    _selectedIndex = 0;
    _windowStart = 0;
    _render();
}

void UsersScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void UsersScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Users");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    for (int i = 0; i < VISIBLE_COUNT; i++)
    {
        int itemIndex = _windowStart + i;
        if (itemIndex >= _screenManager.getEmployeeStore().count())
            break;

        int y = ITEM_Y0 + i * ITEM_HEIGHT;
        if (itemIndex == _selectedIndex)
        {
            _displayManager.fillRect(0, y - 1, 120, ITEM_HEIGHT - 1, SSD1306_WHITE);
            _displayManager.setTextColor(SSD1306_BLACK);
        }
        else
        {
            _displayManager.setTextColor(SSD1306_WHITE);
        }
        _displayManager.print(4, y, _screenManager.getEmployeeStore().at(itemIndex).name);
    }

    _displayManager.setTextColor(SSD1306_WHITE);
    _drawScrollbar();

    _displayManager.print(89, 53, "Exit");
    _displayManager.update();
}

void UsersScreen::_drawScrollbar()
{
    const int trackX = 125;
    const int trackY = ITEM_Y0;
    const int trackH  = VISIBLE_COUNT * ITEM_HEIGHT - 1;

    _displayManager.drawLine(trackX, trackY, trackX, trackY + trackH, SSD1306_WHITE);

    int thumbH = trackH * VISIBLE_COUNT / _screenManager.getEmployeeStore().count();
    if (thumbH < 3) thumbH = 3;

    int maxStart = _screenManager.getEmployeeStore().count() - VISIBLE_COUNT;
    int thumbY = trackY;
    if (maxStart > 0)
        thumbY = trackY + (trackH - thumbH) * _windowStart / maxStart;

    _displayManager.fillRect(trackX - 1, thumbY, 3, thumbH, SSD1306_WHITE);
}

void UsersScreen::loop()
{
    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        if (_selectedIndex > 0)
        {
            _selectedIndex--;
            if (_selectedIndex < _windowStart)
                _windowStart--;
            _render();
        }
        break;
    case Button::DOWN:
        if (_selectedIndex < _screenManager.getEmployeeStore().count() - 1)
        {
            _selectedIndex++;
            if (_selectedIndex >= _windowStart + VISIBLE_COUNT)
                _windowStart++;
            _render();
        }
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::EMPLOYEE);
        break;
    default:
        break;
    }
}
