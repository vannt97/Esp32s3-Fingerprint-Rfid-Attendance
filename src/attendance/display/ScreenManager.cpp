#include "ScreenManager.h"

// ── Constructor nhận DisplayManager ───────────
ScreenManager::ScreenManager(DisplayManager &displayManager, TimeManager &tm, WifiManager &wm, ButtonManager &bm)
    : _displayManager(displayManager),
      _timeManager(tm),
      _wifiManager(wm),
      _buttonManager(bm),
      _menuScreen(*this), // truyền ScreenManager vào MenuScreen
      _homeScreen(*this),
      _connectingWifiScreen(*this),
      _boardInfoScreen(*this),
      _wifiManagerScreen(*this),
      _statusScreen(*this),
      _weatherScreen(*this),
      _signalBlockScreen(*this),
      _gameScreen(*this),
      _employeeScreen(*this),
      _enrollScreen(*this),
      _fingerSelectScreen(*this),
      _fingerScanScreen(*this)

{
}

// ── Hàm chung chuyển screen ───────────────────
void ScreenManager::_switchTo(IScreen *screen)
{
    if (_currentScreen)
        _currentScreen->onExit();
    _currentScreen = screen;
    if (_currentScreen)
        _currentScreen->onEnter();
}

void ScreenManager::showScreen(ScreenId id)
{
    IScreen* target = nullptr;
    switch (id) {
        case ScreenId::HOME:            target = &_homeScreen;            break;
        case ScreenId::CONNECTING_WIFI: target = &_connectingWifiScreen;  break;
        case ScreenId::MENU:            target = &_menuScreen;            break;
        case ScreenId::BOARD_INFO:      target = &_boardInfoScreen;       break;
        case ScreenId::WIFI_MANAGER:    target = &_wifiManagerScreen;     break;
        case ScreenId::STATUS:          target = &_statusScreen;          break;
        case ScreenId::WEATHER:         target = &_weatherScreen;         break;
        case ScreenId::SIGNAL_BLOCK:    target = &_signalBlockScreen;     break;
        case ScreenId::GAME:            target = &_gameScreen;            break;
        case ScreenId::EMPLOYEE:        target = &_employeeScreen;        break;
        case ScreenId::ENROLL:          target = &_enrollScreen;          break;
        case ScreenId::FINGER_SELECT:   target = &_fingerSelectScreen;    break;
        case ScreenId::FINGER_SCAN:     target = &_fingerScanScreen;      break;
    }
    _switchTo(target);
}

IScreen *ScreenManager::getCurrentScreen()
{
    return _currentScreen;
}

void ScreenManager::loop()
{
    if (_currentScreen)
        _currentScreen->loop();
}
