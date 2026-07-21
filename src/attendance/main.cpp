#include <Arduino.h>
#include "config.h"
#include "display/DisplayManager.h"
#include "display/ScreenManager.h"
#include "input/ButtonManager.h"
#include "services/WifiManager.h"
#include "services/TimeManager.h"
#include "services/FingerprintService.h"
#include "services/RfidService.h"

// ── Khai báo ──────────────────────────────────
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
ScreenManager *screenManager = nullptr;
TimeManager timeManager;
ButtonManager buttonManager;
FingerprintService fingerprintService;
RfidService rfidService;
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

    bool fpOk = fingerprintService.begin();
    bool rfidOk = rfidService.begin();
    Serial.printf("Fingerprint sensor: %s\n", fpOk ? "OK" : "NOT FOUND");
    Serial.printf("RFID reader: %s\n", rfidOk ? "OK" : "NOT FOUND");

    screenManager = new ScreenManager(DisplayManager::getInstance(), timeManager, wifiManager, buttonManager,
                                       fingerprintService, rfidService);

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