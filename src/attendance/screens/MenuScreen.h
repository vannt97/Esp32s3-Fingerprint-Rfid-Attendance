#pragma once
#include "assets/bitmaps.h"
#include "IScreen.h"
#include "ScreenData.h"
#include "ScreenId.h"
class ScreenManager;

struct MenuItem
{
    const unsigned char *icon;
    int iconWidth;
    int iconHeight;
    int yOffsetTextPos;
    int yOffsetIconPos;
    const char *name;
    ScreenId screenId;
};
extern const MenuItem MENU_ITEMS[];  // chỉ khai báo, không định nghĩa
extern const int MENU_ITEM_COUNT;


class MenuScreen : public IScreen
{
public:
    explicit MenuScreen(ScreenManager &sm);

    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    ScreenManager &_screenManager;
    DisplayManager &_displayManager;
    int _currentScrollPos = INIT_ITEM_POS;
    int _currentMenuItemIndex = 0;
    int _INIT_ACTIVE_BORDER_Y_POS = 2;
    int _activeBorderYPos = _INIT_ACTIVE_BORDER_Y_POS;
    int _activeBorderTravelDistance = 24;
    void _drawScrollThumb();
    void _drawScrollTrack();
    void _removeScrollTrack();
    void _redrawScroll();
    void _handleScrollDown();
    void _handleScrollUp();
    void _drawActiveBorder();
    void _drawScrollDownMenuItem();
    void _drawScrollUpMenuItem();
    void _removeMenuUI();
    void _drawMenuItem(int index, int yPos);
    void _drawTwoItemInMenu();
    void _drawScrollDown();
    void _drawScrollUp();
};