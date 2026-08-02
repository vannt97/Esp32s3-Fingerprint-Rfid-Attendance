#pragma once
#include "IScreen.h"

class ScreenManager;

class EmployeeScreen : public IScreen
{
public:
    explicit EmployeeScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    void _render();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    int _menuIdx = 0;
    static constexpr const char* ITEM_NAMES[] = {"Enroll", "Users", "Delete"};
    static constexpr int         ITEM_COUNT   = 3;
};
