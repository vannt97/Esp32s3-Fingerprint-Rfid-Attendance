#include "MenuScreen.h"
#include "display/ScreenManager.h"

const MenuItem MENU_ITEMS[] = {
    {image_smartphone_bits,          9,  16, 4,  0,  "Board Info",    ScreenId::BOARD_INFO},
    {image_tv_bits,                  17, 16, 4,  0,  "Status",        ScreenId::STATUS},
    {image_wifi_full_bits,           19, 16, 4,  0,  "Wi-Fi List",    ScreenId::WIFI_MANAGER},
    {image_wifi_not_connected_bits,  19, 16, 4,  0,  "Block Signals", ScreenId::SIGNAL_BLOCK},
    {image_weather_cloud_sunny_bits, 17, 16, 6,  0,  "Weather",       ScreenId::WEATHER},
    {image_bike_bits,                17, 16, 4, -2,  "Game",          ScreenId::GAME},
};

const int MENU_ITEM_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

MenuScreen::MenuScreen(ScreenManager &screenManager)
    : _screenManager(screenManager),
      _displayManager(screenManager.getDisplayManager())
{
}

void MenuScreen::_drawScrollThumb()
{
    _displayManager.fillRect(121, _currentScrollPos, SCROLL_THUMB_WIDTH, SCROLL_THUMB_HEIGHT, SSD1306_WHITE);
}

void MenuScreen::_drawScrollTrack()
{
    _displayManager.drawBitmap(123, 3, image_scroll_track_bits, 1, 47, 1);
}

void MenuScreen::_removeScrollTrack()
{
    _displayManager.fillRect(121, 3, 6, 55, SSD1306_BLACK);
}

void MenuScreen::_removeMenuUI()
{
    _displayManager.fillRect(0, 0, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SSD1306_BLACK);
}

void MenuScreen::_drawActiveBorder()
{
    _displayManager.drawRoundRect(3, _activeBorderYPos, ACTIVE_BORDER_WIDTH, ACTIVE_BORDER_HEIGHT, 4, SSD1306_WHITE);
}

void MenuScreen::_redrawScroll()
{
    _removeMenuUI();

    _removeScrollTrack();

    _drawTwoItemInMenu();

    _drawScrollTrack();

    _drawScrollThumb();

    _drawActiveBorder();

    _displayManager.update();
}

void MenuScreen::_handleScrollDown()
{
    _currentScrollPos += TRAVEL_DISTANCE;
    if (_currentScrollPos > SCROLL_END_POINT)
        _currentScrollPos = INIT_ITEM_POS;
}

void MenuScreen::_handleScrollUp()
{
    _currentScrollPos -= TRAVEL_DISTANCE;
    if (_currentScrollPos < INIT_ITEM_POS)
        _currentScrollPos = SCROLL_END_POINT;
}

void MenuScreen::_drawScrollDownMenuItem()
{
    _currentMenuItemIndex += 1;
    if (_currentMenuItemIndex >= MENU_ITEM_COUNT)
        _currentMenuItemIndex = 0;

    _activeBorderYPos = (_currentMenuItemIndex % 2 != 0)
                            ? _INIT_ACTIVE_BORDER_Y_POS + _activeBorderTravelDistance
                            : _INIT_ACTIVE_BORDER_Y_POS;
}

void MenuScreen::_drawScrollUpMenuItem()
{
    _currentMenuItemIndex -= 1;
    if (_currentMenuItemIndex < 0)
        _currentMenuItemIndex = MENU_ITEM_COUNT - 1;

    _activeBorderYPos = (_currentMenuItemIndex % 2 != 0)
                            ? _INIT_ACTIVE_BORDER_Y_POS + _activeBorderTravelDistance
                            : _INIT_ACTIVE_BORDER_Y_POS;
}

void MenuScreen::_drawMenuItem(int index, int yPos)
{
    if (index >= MENU_ITEM_COUNT)
        return;
    const MenuItem &item = MENU_ITEMS[index];
    _displayManager.drawCenteredWithBitmap(yPos + item.yOffsetIconPos, yPos + item.yOffsetTextPos, item.icon, item.iconWidth, item.iconHeight, item.name);
}

void MenuScreen::_drawTwoItemInMenu()
{
    int page = _currentMenuItemIndex / 2;
    int firstIndex = page * 2;

    _drawMenuItem(firstIndex, ITEM_Y_POS_1);     // vị trí item trên
    _drawMenuItem(firstIndex + 1, ITEM_Y_POS_2); // vị trí item dưới
}

void MenuScreen::_drawScrollDown()
{
    _handleScrollDown();
    _drawScrollDownMenuItem();
    _redrawScroll();
}

void MenuScreen::_drawScrollUp()
{
    _handleScrollUp();
    _drawScrollUpMenuItem();
    _redrawScroll();
}

void MenuScreen::onEnter()
{
    _displayManager.clear();

    _drawActiveBorder();

    _drawScrollTrack();

    _drawScrollThumb();

    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);
    _displayManager.print(13, 53, "Select");
    _displayManager.print(89, 53, "Exit");

    _drawTwoItemInMenu();

    _displayManager.update();
}

void MenuScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void MenuScreen::loop()
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
    {
        const MenuItem &item = MENU_ITEMS[_currentMenuItemIndex];
        _screenManager.showScreen(item.screenId);
        break;
    }
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::HOME);
        break;
    case Button::NONE:
        break;
    }
}