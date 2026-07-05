#include <Arduino.h>
#include "config.h"
#include "display/DisplayManager.h"
#include "display/ScreenManager.h"
#include "input/ButtonManager.h"
#include "services/WifiManager.h"
#include "services/TimeManager.h"

// ── Khai báo ──────────────────────────────────
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
ScreenManager *screenManager = nullptr;
TimeManager timeManager;
ButtonManager buttonManager;
Timeout wifiTimer(CONNECT_WIFI_TIMEOUT);
Timeout setupTimeTimer(TIME_TIMEOUT);

void setup()
{
    Serial.begin(115200);

    if (!DisplayManager::getInstance().begin())
    {
        return;
    }
    wifiManager.begin();
    buttonManager.begin();
    screenManager = new ScreenManager(DisplayManager::getInstance(), timeManager, wifiManager, buttonManager);

    while (wifiManager.isConnecting() && wifiTimer.isRunning())
    {
        wifiManager.connecting();
        screenManager->showScreen(ScreenId::CONNECTING_WIFI);
    }

    if (wifiManager.isConnected())
    {
        timeManager.begin();
    }

    screenManager->showScreen(ScreenId::HOME);
    Serial.println("Setup completed.");
    // screenManager->showScreen(ScreenId::BOARD_INFO);
}

void loop()
{
    screenManager->loop();
}