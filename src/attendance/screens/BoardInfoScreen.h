#pragma once
#include "IScreen.h"
#include "ScreenData.h"
#include "assets/bitmaps.h"
#include "assets/animations.h"
#include "helpers/TimeoutHelper.h"
#include <vector>
#include <string>

class ScreenManager; // forward declare, KHÔNG include ScreenManager.h

class BoardInfoScreen : public IScreen
{
public:
    explicit BoardInfoScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    ScreenManager &_screenManager;
    DisplayManager &_displayManager;
    int _currentY = 0;
    std::vector<std::string> _lines;
    void _drawScrollDown();
    void _drawScrollUp();
    void _render();
};