#pragma once
#include "IScreen.h"

class ScreenManager;

class SignalBlockScreen : public IScreen
{
public:
    explicit SignalBlockScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;
    bool _isBlocking = false;
};