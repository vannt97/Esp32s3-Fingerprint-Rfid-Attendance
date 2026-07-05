#include "SignalBlockScreen.h"
#include "display/ScreenManager.h"

SignalBlockScreen::SignalBlockScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

// void SignalBlockScreen::_render()
// {
//     _displayManager.clear();
//     _displayManager.setTextColor(1);
//     _displayManager.setTextWrap(false);

//     _displayManager.print(0, 0, "Signal Block");
//     _displayManager.print(0, 20, _isBlocking ? "Status: ON " : "Status: OFF");
//     _displayManager.print(0, 40, "[SEL] toggle");
//     _displayManager.print(0, 53, "[EXIT] back");
//     _displayManager.update();
// }

void SignalBlockScreen::onEnter()
{
    _isBlocking = false;
    // _render();
     _displayManager.clear();
    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Signal Block");
    _displayManager.print(0, 20, _isBlocking ? "Status: ON " : "Status: OFF");
    _displayManager.print(0, 40, "[SEL] toggle");
    _displayManager.print(0, 53, "[EXIT] back");
    _displayManager.update();
}

void SignalBlockScreen::onExit()
{
    // Đảm bảo tắt jam trước khi rời screen
    if (_isBlocking)
    {
        _isBlocking = false;
        // TODO: sm.getWifiManager().stopBlock();
    }
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void SignalBlockScreen::loop()
{
    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::SELECT:
        _isBlocking = !_isBlocking;
        // TODO: _isBlocking ? sm.getWifiManager().startBlock() : stop
        // _render();
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    default:
        break;
    }
}