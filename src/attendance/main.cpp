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
#include "services/AttendanceLog.h"
#include "services/AudioFeedback.h"
#include "services/EmployeeStore.h"
#include "services/ApiService.h"
#include "screens/FingerNames.h"

// ── Khai báo ──────────────────────────────────
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
ScreenManager *screenManager = nullptr;
TimeManager timeManager;
ButtonManager buttonManager;
FingerprintService fingerprintService;
RfidService rfidService;
EnrollmentStore enrollmentStore;
AttendanceLog attendanceLog;
AudioFeedback audioFeedback;
EmployeeStore employeeStore;
ApiService apiService;
Timeout wifiTimer(CONNECT_WIFI_TIMEOUT);
Timeout setupTimeTimer(TIME_TIMEOUT);
Timeout attendanceSyncTimer(API_ATTENDANCE_SYNC_INTERVAL_MS);
Timeout employeeRefetchTimer(API_EMPLOYEE_REFETCH_INTERVAL_MS);
bool timeSyncStarted = false;

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
    attendanceLog.begin();
    audioFeedback.begin();
    employeeStore.begin();
    apiService.begin();

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);

    // Dump tạm để verify mapping còn sống sau reset (bỏ khi đã có màn hình xem thật).
    Serial.println("=== Enrollment mappings ===");
    for (int i = 0; i < employeeStore.count(); i++)
    {
        const Employee &emp = employeeStore.at(i);
        String cardUid;
        if (enrollmentStore.getCardMapping(emp.id, cardUid))
            Serial.printf("  %s (id=%u): card=%s\n", emp.name, emp.id, cardUid.c_str());

        for (int f = 0; f < FINGER_COUNT; f++)
        {
            uint16_t templateId;
            if (enrollmentStore.getFingerMapping(emp.id, f, templateId))
                Serial.printf("  %s (id=%u): finger[%d]=template#%u\n", emp.name, emp.id, f, templateId);
        }
    }
    Serial.println("============================");

    Serial.println("=== Attendance log ===");
    attendanceLog.dumpToSerial();
    Serial.println("======================");

    screenManager = new ScreenManager(DisplayManager::getInstance(), timeManager, wifiManager, buttonManager,
                                       fingerprintService, rfidService, enrollmentStore, attendanceLog,
                                       audioFeedback, employeeStore, apiService);

    while (wifiManager.isConnecting() && wifiTimer.isRunning())
    {
        wifiManager.connecting();
        screenManager->showScreen(ScreenId::CONNECTING_WIFI);
    }

    if (wifiManager.isConnected())
    {
        timeManager.begin();
        timeSyncStarted = true;

        bool empOk = apiService.fetchEmployees(employeeStore);
        Serial.printf("Employee sync: %s (%d employees)\n", empOk ? "OK" : "failed (dùng cache/mock)",
                      employeeStore.count());
    }

    screenManager->showScreen(ScreenId::HOME);
    Serial.println("Setup completed.");
    // screenManager->showScreen(ScreenId::BOARD_INFO);
}

void loop()
{
    wifiManager.loop();

    if (!timeSyncStarted && wifiManager.isConnected())
    {
        timeManager.begin();
        timeSyncStarted = true;
    }

    if (wifiManager.isConnected() && attendanceSyncTimer.isExpired())
        apiService.syncPendingAttendance(attendanceLog);

    if (wifiManager.isConnected() && employeeRefetchTimer.isExpired())
        apiService.fetchEmployees(employeeStore);

    audioFeedback.loop();
    screenManager->loop();
}