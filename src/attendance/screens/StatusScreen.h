#pragma once
#include "IScreen.h"
#include "services/SystemInfoService.h"

class ScreenManager;

class StatusScreen : public IScreen
{
public:
    explicit StatusScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    void _render();
    void _renderTimeSection(int y);
    void _renderWifiSection(int y, const WifiInfo &wifi);
    void _renderSystemSection(int y, const SystemStatus &sys);

    ScreenManager        &_screenManager;
    DisplayManager       &_displayManager;
    SystemInfoService     _infoService;   // service inject thẳng

    unsigned long _lastRefresh = 0;
    static constexpr unsigned long REFRESH_MS = 2000;
};