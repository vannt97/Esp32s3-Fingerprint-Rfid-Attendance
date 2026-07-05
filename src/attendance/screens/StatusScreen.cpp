#include "StatusScreen.h"
#include "display/ScreenManager.h"
#include "utils/DisplayUtils.h"
#include <WiFi.h>
#include <esp_system.h> // esp_get_free_heap_size()

StatusScreen::StatusScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

// ── Public ────────────────────────────────────────────────

void StatusScreen::onEnter()
{
    _lastRefresh = 0; // force render ngay lập tức
    _render();
}

void StatusScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void StatusScreen::loop()
{
    // Auto-refresh
    if (millis() - _lastRefresh >= REFRESH_MS)
        _render();

    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::SELECT:
        _render(); // manual refresh
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    default:
        break;
    }
}

// ── Private — render ──────────────────────────────────────
void StatusScreen::_render()
{
    _lastRefresh = millis();

    // Screen chỉ biết: lấy data → render, không biết data đến từ đâu
    SystemInfo info = _infoService.getInfo();

    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _renderTimeSection(0);
    _renderWifiSection(22, info.wifi);
    _renderSystemSection(44, info.system);

    _displayManager.update();
}

void StatusScreen::_renderWifiSection(int y, const WifiInfo &wifi)
{
    _displayManager.print(0, y, "WiFi:");

    if (!wifi.connected)
    {
        _displayManager.print(30, y, "Disconnected");
        return;
    }

    String ssid = wifi.ssid;
    if (ssid.length() > 10)
        ssid = ssid.substring(0, 9) + "~";

    _displayManager.print(30, y, ssid);
    _displayManager.print(110, y, rssiToBarString(wifi.rssi));
    _displayManager.print(0, y + 10, wifi.ip);
    _displayManager.print(90, y + 10, String(wifi.rssi) + "dB");
}

void StatusScreen::_renderSystemSection(int y, const SystemStatus &sys)
{
    String mem = formatBytes(sys.usedHeap) + "/" + formatBytes(sys.totalHeap);
    _displayManager.print(0, y, "Mem:" + mem);
    _displayManager.print(0, y + 10, "Up:" + formatUptime(sys.uptimeMs));
}

void StatusScreen::_renderTimeSection(int y)
{
    TimeStringData td = _screenManager.getTimeManager().getTimeAndDate();

    _displayManager.print(0,  y, td.date);  // "21/05/2025"
    _displayManager.print(80, y, td.time);  // "14:32"

    _displayManager.drawLine(0, y + 10, SCREEN_WIDTH, y + 10, SSD1306_WHITE);
}