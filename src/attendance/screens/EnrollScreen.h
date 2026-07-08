#pragma once
#include "IScreen.h"

class ScreenManager;

class EnrollScreen : public IScreen
{
public:
    explicit EnrollScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    void _render();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    int _menuIdx = 0;
    static constexpr const char* ITEM_NAMES[] = {"Fingerprint", "Card Number"};
    static constexpr int         ITEM_COUNT   = 2;
};
