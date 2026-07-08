#include "FingerSelectScreen.h"
#include "display/ScreenManager.h"

FingerSelectScreen::FingerSelectScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void FingerSelectScreen::onEnter()
{
    _selectedIndex = 0;
    _windowStart = 0;
    _render();
}

void FingerSelectScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void FingerSelectScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Fingerprint");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    for (int i = 0; i < VISIBLE_COUNT; i++)
    {
        int itemIndex = _windowStart + i;
        if (itemIndex >= FINGER_COUNT)
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
        _displayManager.print(4, y, FINGER_NAMES[itemIndex]);
    }

    _displayManager.setTextColor(SSD1306_WHITE);
    _drawScrollbar();

    _displayManager.print(13, 53, "Select");
    _displayManager.print(89, 53, "Exit");
    _displayManager.update();
}

void FingerSelectScreen::_drawScrollbar()
{
    const int trackX = 125;
    const int trackY = ITEM_Y0;
    const int trackH  = VISIBLE_COUNT * ITEM_HEIGHT - 1;

    _displayManager.drawLine(trackX, trackY, trackX, trackY + trackH, SSD1306_WHITE);

    int thumbH = trackH * VISIBLE_COUNT / FINGER_COUNT;
    if (thumbH < 3) thumbH = 3;

    int maxStart = FINGER_COUNT - VISIBLE_COUNT;
    int thumbY = trackY;
    if (maxStart > 0)
        thumbY = trackY + (trackH - thumbH) * _windowStart / maxStart;

    _displayManager.fillRect(trackX - 1, thumbY, 3, thumbH, SSD1306_WHITE);
}

void FingerSelectScreen::loop()
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
        if (_selectedIndex < FINGER_COUNT - 1)
        {
            _selectedIndex++;
            if (_selectedIndex >= _windowStart + VISIBLE_COUNT)
                _windowStart++;
            _render();
        }
        break;
    case Button::SELECT:
        _screenManager.setSelectedFingerIndex(_selectedIndex);
        _screenManager.showScreen(ScreenId::FINGER_SCAN);
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::ENROLL);
        break;
    default:
        break;
    }
}
