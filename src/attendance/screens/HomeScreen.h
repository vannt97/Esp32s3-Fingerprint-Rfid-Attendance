#pragma once
#include "IScreen.h"
#include "ScreenData.h"
#include "assets/bitmaps.h"
#include "helpers/TimeoutHelper.h"

class ScreenManager; // forward declare, KHÔNG include ScreenManager.h
class HomeScreen : public IScreen
{
public:
    explicit HomeScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    void _drawTimeOnly();
    HomeData _data = {
        .time = "--:--",
        .date = "--/--/----",
        .batteryPercent = 85,
        .wifiConnected = false};

    Timeout _getTimeTimer{GET_TIME_TIMEOUT};
    ScreenManager &_screenManager;
    DisplayManager &_displayManager;
};