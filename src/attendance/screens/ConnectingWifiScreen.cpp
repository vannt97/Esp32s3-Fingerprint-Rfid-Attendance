
#include "ConnectingWifiScreen.h"
#include "display/ScreenManager.h"

ConnectingWifiScreen::ConnectingWifiScreen(ScreenManager &sm) : _screenManager(sm), _displayManager(sm.getDisplayManager()), _frameTimer(FRAME_DELAY_MS) {}
void ConnectingWifiScreen::onEnter()
{
    if (!_frameTimer.isExpired())
        return;
    _currentFrame = (_currentFrame + 1) % wifi_all_array_count;
    _displayManager.clear();
    _displayManager.drawBitmap(55, 19, wifi_all_array[_currentFrame], 19, 16, 1);
    _displayManager.print(23, 39, "Connecting...");
    _displayManager.update();
}