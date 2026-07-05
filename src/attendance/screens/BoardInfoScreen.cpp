#include "BoardInfoScreen.h"
#include "services/ChipInfo.h"
#include "display/ScreenManager.h"

BoardInfoScreen::BoardInfoScreen(ScreenManager &sm) : _screenManager(sm), _displayManager(sm.getDisplayManager()) {}

void BoardInfoScreen::_drawScrollDown()
{
    _currentY = min(_currentY + LINE_HEIGHT, (int)(_lines.size() * LINE_HEIGHT - SCREEN_HEIGHT));
    _render();
}

void BoardInfoScreen::_drawScrollUp()
{
    _currentY = max(_currentY - LINE_HEIGHT, 0);
    _render();
}

void BoardInfoScreen::onExit()
{
}

void BoardInfoScreen::onEnter()
{
    

    _currentY = 0;
    _lines = ChipInfo::getAllInfo();

    _render();
}

void BoardInfoScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(1);
    _displayManager.setTextSize(1);
    
    _displayManager.print(89, 53, "Exit");

    for (int i = 0; i < _lines.size(); i++)
    {
        int y = i * LINE_HEIGHT - _currentY;
        if (y >= 0 && y < SCREEN_HEIGHT)
        {
            _displayManager.print(0, y, _lines[i].c_str());
        }
    }
    _displayManager.update();
}

void BoardInfoScreen::loop()
{

    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        _drawScrollUp();
        break;
    case Button::DOWN:
        _drawScrollDown();
        break;
    case Button::SELECT:
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    case Button::NONE:
        break;
    }
}