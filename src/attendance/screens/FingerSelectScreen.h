#pragma once
#include "IScreen.h"
#include "FingerNames.h"

class ScreenManager;

class FingerSelectScreen : public IScreen
{
public:
    explicit FingerSelectScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    void _render();
    void _drawScrollbar();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    int _selectedIndex = 0;
    int _windowStart   = 0;

    static constexpr int VISIBLE_COUNT = 3;
    static constexpr int ITEM_Y0       = 16;
    static constexpr int ITEM_HEIGHT   = 11;
};
