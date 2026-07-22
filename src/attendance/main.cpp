#include <Arduino.h>
#include "config.h"
#include "display/DisplayManager.h"
#include "display/ScreenManager.h"
#include "input/ButtonManager.h"
#include "services/WifiManager.h"
#include "services/TimeManager.h"
#include "services/FingerprintService.h"
#include "services/RfidService.h"
#include "services/EnrollmentStore.h"
#include "screens/EmployeeData.h"
#include "screens/FingerNames.h"

// ── Khai báo ──────────────────────────────────
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
ScreenManager *screenManager = nullptr;
TimeManager timeManager;
ButtonManager buttonManager;
FingerprintService fingerprintService;
RfidService rfidService;
EnrollmentStore enrollmentStore;
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
    enrollmentStore.begin();

    // Dump tạm để verify mapping còn sống sau reset (bỏ khi đã có màn hình xem thật).
    Serial.println("=== Enrollment mappings ===");
    for (int i = 0; i < EMPLOYEE_COUNT; i++)
    {
        uint16_t id = EMPLOYEES[i].id;
        String cardUid;
        if (enrollmentStore.getCardMapping(id, cardUid))
            Serial.printf("  %s (id=%u): card=%s\n", EMPLOYEES[i].name, id, cardUid.c_str());

        for (int f = 0; f < FINGER_COUNT; f++)
        {
            uint16_t templateId;
            if (enrollmentStore.getFingerMapping(id, f, templateId))
                Serial.printf("  %s (id=%u): finger[%d]=template#%u\n", EMPLOYEES[i].name, id, f, templateId);
        }
    }
    Serial.println("============================");

    screenManager = new ScreenManager(DisplayManager::getInstance(), timeManager, wifiManager, buttonManager,
                                       fingerprintService, rfidService, enrollmentStore);

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