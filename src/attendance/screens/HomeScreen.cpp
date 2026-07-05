#include "HomeScreen.h"
#include "display/ScreenManager.h"
HomeScreen::HomeScreen(ScreenManager &screenManager)
    : _screenManager(screenManager),
      _displayManager(screenManager.getDisplayManager())
{
}
void HomeScreen::onEnter()
{
    auto td = _screenManager.getTimeManager().getTimeAndDate();
    _data.time = td.time;
    _data.date = td.date;
    _screenManager.getWifiManager().isConnected() ? _data.wifiConnected = true : _data.wifiConnected = false;

    _displayManager.clear();

    // battery_charger_connected
    _displayManager.drawBitmap(100, 4, image_battery_charger_connected_bits, 24, 16, 1);

    // Menu
    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);
    _displayManager.print(14, 53, "Menu");

    // Time
    _displayManager.setTextSize(2);
    _displayManager.print(35, 24, _data.time);

    // wifi_full
    if (_data.wifiConnected)
    {
        _displayManager.drawBitmap(5, 4, image_wifi_full_bits, 19, 16, 1);
    }
    else
    {
        _displayManager.drawBitmap(5, 4, image_wifi_not_connected_bits, 19, 16, 1);
    }

    // Date
    _displayManager.setTextSize(1);
    _displayManager.print(35, 42, _data.date);

    _displayManager.update();
}

void HomeScreen::_drawTimeOnly()
{
    // Xóa vùng time cũ (x, y, width, height)
    _displayManager.fillRect(35, 24, 58, 16, SSD1306_BLACK); // vùng time "HH:MM" size 2
    _displayManager.fillRect(35, 42, 58, 8, SSD1306_BLACK);  // vùng date "DD/MM/YYYY" size 1

    _displayManager.setTextSize(2);
    _displayManager.print(35, 24, _data.time);
    _displayManager.setTextSize(1);
    _displayManager.print(35, 42, _data.date);
    _displayManager.update();
}

void HomeScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void HomeScreen::loop()
{
    if (_getTimeTimer.isExpired())
    {
        auto td = _screenManager.getTimeManager().getTimeAndDate();
        _screenManager.getWifiManager().isConnected() ? _data.wifiConnected = true : _data.wifiConnected = false;
        _data.time = td.time;
        _data.date = td.date;
        _drawTimeOnly();
    }

    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        break;
    case Button::DOWN:
        break;
    case Button::LEFT:
        break;
    case Button::RIGHT:
        break;
    case Button::SELECT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    case Button::EXIT:
        break;
    case Button::NONE:
        break;
    }
}