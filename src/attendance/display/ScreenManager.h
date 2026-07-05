#pragma once
#include "DisplayManager.h"
#include "screens/IScreen.h"
#include "screens/HomeScreen.h"
#include "screens/MenuScreen.h"
#include "screens/ConnectingWifiScreen.h"
#include "screens/WifiManagerScreen.h"
#include "screens/ScreenData.h"
#include "screens/ScreenId.h"
#include "input/ButtonManager.h"
#include "services/WifiManager.h"
#include "services/TimeManager.h"
#include "screens/BoardInfoScreen.h"
#include "screens/StatusScreen.h"
#include "screens/WeatherScreen.h"
#include "screens/SignalBlockScreen.h"
#include "screens/GameScreen.h"
class ScreenManager
{
public:
    explicit ScreenManager(DisplayManager &displayManager, TimeManager &tm, WifiManager &wm, ButtonManager &bm);

    void showScreen(ScreenId id);

    DisplayManager &getDisplayManager() { return _displayManager; }
    TimeManager &getTimeManager() { return _timeManager; }
    ButtonManager &getButtonManager() { return _buttonManager; }
    WifiManager &getWifiManager() { return _wifiManager; }
    IScreen *getCurrentScreen();
    void loop();

private:
    void _switchTo(IScreen *screen);

    DisplayManager &_displayManager;
    TimeManager &_timeManager;
    WifiManager &_wifiManager;
    ButtonManager &_buttonManager;
    IScreen *_currentScreen = nullptr;

    HomeScreen _homeScreen;
    ConnectingWifiScreen _connectingWifiScreen;
    MenuScreen _menuScreen;
    BoardInfoScreen _boardInfoScreen;
    WifiManagerScreen _wifiManagerScreen;
    StatusScreen _statusScreen;
    WeatherScreen _weatherScreen;
    SignalBlockScreen _signalBlockScreen;
    GameScreen _gameScreen;
};