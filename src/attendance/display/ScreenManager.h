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
#include "services/FingerprintService.h"
#include "services/RfidService.h"
#include "services/EnrollmentStore.h"
#include "services/AttendanceLog.h"
#include "services/AudioFeedback.h"
#include "services/EmployeeStore.h"
#include "services/ApiService.h"
#include "screens/EnrollTarget.h"
#include "screens/EmployeeSelectScreen.h"
#include "screens/BoardInfoScreen.h"
#include "screens/StatusScreen.h"
#include "screens/WeatherScreen.h"
#include "screens/SignalBlockScreen.h"
#include "screens/GameScreen.h"
#include "screens/EmployeeScreen.h"
#include "screens/EnrollScreen.h"
#include "screens/FingerSelectScreen.h"
#include "screens/FingerScanScreen.h"
#include "screens/UsersScreen.h"
#include "screens/CardScanScreen.h"
#include "screens/DeleteEnrollmentScreen.h"
class ScreenManager
{
public:
    explicit ScreenManager(DisplayManager &displayManager, TimeManager &tm, WifiManager &wm, ButtonManager &bm,
                            FingerprintService &fps, RfidService &rs, EnrollmentStore &es, AttendanceLog &al,
                            AudioFeedback &af, EmployeeStore &emps, ApiService &api);

    void showScreen(ScreenId id);

    DisplayManager &getDisplayManager() { return _displayManager; }
    TimeManager &getTimeManager() { return _timeManager; }
    ButtonManager &getButtonManager() { return _buttonManager; }
    WifiManager &getWifiManager() { return _wifiManager; }
    FingerprintService &getFingerprintService() { return _fingerprintService; }
    RfidService &getRfidService() { return _rfidService; }
    EnrollmentStore &getEnrollmentStore() { return _enrollmentStore; }
    AttendanceLog &getAttendanceLog() { return _attendanceLog; }
    AudioFeedback &getAudioFeedback() { return _audioFeedback; }
    EmployeeStore &getEmployeeStore() { return _employeeStore; }
    ApiService &getApiService() { return _apiService; }
    IScreen *getCurrentScreen();
    void loop();

    void setSelectedFingerIndex(int idx) { _selectedFingerIndex = idx; }
    int getSelectedFingerIndex() const { return _selectedFingerIndex; }

    void setSelectedEmployeeId(uint16_t id) { _selectedEmployeeId = id; }
    uint16_t getSelectedEmployeeId() const { return _selectedEmployeeId; }

    void setPendingEnrollTarget(EnrollTarget t) { _pendingEnrollTarget = t; }
    EnrollTarget getPendingEnrollTarget() const { return _pendingEnrollTarget; }

private:
    void _switchTo(IScreen *screen);

    DisplayManager &_displayManager;
    TimeManager &_timeManager;
    WifiManager &_wifiManager;
    ButtonManager &_buttonManager;
    FingerprintService &_fingerprintService;
    RfidService &_rfidService;
    EnrollmentStore &_enrollmentStore;
    AttendanceLog &_attendanceLog;
    AudioFeedback &_audioFeedback;
    EmployeeStore &_employeeStore;
    ApiService &_apiService;
    IScreen *_currentScreen = nullptr;
    int _selectedFingerIndex = 0;
    uint16_t _selectedEmployeeId = 0;
    EnrollTarget _pendingEnrollTarget = EnrollTarget::FINGER;

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
    EmployeeSelectScreen _employeeSelectScreen;
    FingerSelectScreen _fingerSelectScreen;
    FingerScanScreen _fingerScanScreen;
    UsersScreen _usersScreen;
    CardScanScreen _cardScanScreen;
    DeleteEnrollmentScreen _deleteEnrollmentScreen;
};