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
#include "screens/EmployeeScreen.h"
#include "screens/EnrollScreen.h"
#include "screens/FingerSelectScreen.h"
#include "screens/FingerScanScreen.h"
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

    void setSelectedFingerIndex(int idx) { _selectedFingerIndex = idx; }
    int getSelectedFingerIndex() const { return _selectedFingerIndex; }

private:
    void _switchTo(IScreen *screen);

    DisplayManager &_displayManager;
    TimeManager &_timeManager;
    WifiManager &_wifiManager;
    ButtonManager &_buttonManager;
    IScreen *_currentScreen = nullptr;
    int _selectedFingerIndex = 0;

    HomeScreen _homeScreen;
    ConnectingWifiScreen _connectingWifiScreen;
    MenuScreen _menuScreen;
    BoardInfoScreen _boardInfoScreen;
    WifiManagerScreen _wifiManagerScreen;
    StatusScreen _statusScreen;
    WeatherScreen _weatherScreen;
    SignalBlockScreen _signalBlockScreen;
    GameScreen _gameScreen;
    EmployeeScreen _employeeScreen;
    EnrollScreen _enrollScreen;
    FingerSelectScreen _fingerSelectScreen;
    FingerScanScreen _fingerScanScreen;
};