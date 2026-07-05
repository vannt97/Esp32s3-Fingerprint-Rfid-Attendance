#pragma once
#include "IScreen.h"
#include "ScreenData.h"
#include "assets/bitmaps.h"
#include "assets/animations.h"
#include "helpers/TimeoutHelper.h"

class ScreenManager; // forward declare, KHÔNG include ScreenManager.h

class ConnectingWifiScreen : public IScreen {
public:
    explicit ConnectingWifiScreen(ScreenManager& sm);
    void onEnter() override;
    void onExit() override {}
    void loop() override {}
private:
    int _currentFrame = 0;
    static const unsigned long FRAME_DELAY_MS = 150; // đổi tốc độ tại đây
    Timeout _frameTimer;
    ScreenManager& _screenManager; 
    DisplayManager& _displayManager;   
};