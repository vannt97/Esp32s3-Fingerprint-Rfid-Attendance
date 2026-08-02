#include "HomeScreen.h"
#include "display/ScreenManager.h"
#include "pins.h"

HomeScreen::HomeScreen(ScreenManager &screenManager)
    : _screenManager(screenManager),
      _displayManager(screenManager.getDisplayManager())
{
}

void HomeScreen::onEnter()
{
    auto td = _screenManager.getTimeManager().getTimeAndDate();
    _data.time = td.time;
    _data.date = td.date;
    _screenManager.getWifiManager().isConnected() ? _data.wifiConnected = true : _data.wifiConnected = false;

    _idleState = IdleState::CLOCK;
    _renderClock();
}

void HomeScreen::_renderClock()
{
    _displayManager.clear();

    // battery_charger_connected
    _displayManager.drawBitmap(100, 4, image_battery_charger_connected_bits, 24, 16, 1);

    // Menu
    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);
    _displayManager.print(14, 53, "Menu");

    // Time
    _displayManager.setTextSize(2);
    _displayManager.print(35, 24, _data.time);

    // wifi_full
    if (_data.wifiConnected)
    {
        _displayManager.drawBitmap(5, 4, image_wifi_full_bits, 19, 16, 1);
    }
    else
    {
        _displayManager.drawBitmap(5, 4, image_wifi_not_connected_bits, 19, 16, 1);
    }

    // Date
    _displayManager.setTextSize(1);
    _displayManager.print(35, 42, _data.date);

    _displayManager.update();
}

void HomeScreen::_drawTimeOnly()
{
    // Xóa vùng time cũ (x, y, width, height)
    _displayManager.fillRect(35, 24, 58, 16, SSD1306_BLACK); // vùng time "HH:MM" size 2
    _displayManager.fillRect(35, 42, 58, 8, SSD1306_BLACK);  // vùng date "DD/MM/YYYY" size 1

    _displayManager.setTextSize(2);
    _displayManager.print(35, 24, _data.time);
    _displayManager.setTextSize(1);
    _displayManager.print(35, 42, _data.date);
    _displayManager.update();
}

void HomeScreen::_renderResult()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);
    _displayManager.setTextSize(1);

    if (_resultName.length() > 0)
        _displayManager.print(4, 20, _resultName);

    _displayManager.print(4, 36, _resultStatus);

    _displayManager.update();
}

void HomeScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void HomeScreen::loop()
{
    if (_idleState == IdleState::RESULT)
    {
        if (_resultTimer.isExpired())
        {
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            _idleState = IdleState::CLOCK;
            _renderClock();
        }
        return;
    }

    if (_getTimeTimer.isExpired())
    {
        auto td = _screenManager.getTimeManager().getTimeAndDate();
        _screenManager.getWifiManager().isConnected() ? _data.wifiConnected = true : _data.wifiConnected = false;
        _data.time = td.time;
        _data.date = td.date;
        _drawTimeOnly();
    }

    _pollAttendance();

    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        break;
    case Button::DOWN:
        break;
    case Button::LEFT:
        break;
    case Button::RIGHT:
        break;
    case Button::SELECT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    case Button::EXIT:
        break;
    case Button::NONE:
        break;
    }
}

void HomeScreen::_pollAttendance()
{
    auto &fp = _screenManager.getFingerprintService();
    uint16_t templateId = 0, confidence = 0;
    FingerVerifyResult result = fp.verifyStep(templateId, confidence);

    if (result == FingerVerifyResult::MATCHED)
    {
        uint16_t employeeId = 0;
        bool found = _screenManager.getEnrollmentStore().findEmployeeByTemplateId(templateId, employeeId);
        _showResult(employeeId, found, 'F');
        return;
    }
    if (result == FingerVerifyResult::NOT_FOUND)
    {
        _showResult(0, false, 'F');
        return;
    }
    // NO_FINGER / ERROR: bỏ qua, không làm gì

    if (_cardPollTimer.isExpired())
    {
        String uid;
        if (_screenManager.getRfidService().pollCard(uid))
        {
            auto &employeeStore = _screenManager.getEmployeeStore();
            uint16_t employeeId = 0;
            bool found = false;
            for (int i = 0; i < employeeStore.count(); i++)
            {
                String stored;
                if (_screenManager.getEnrollmentStore().getCardMapping(employeeStore.at(i).id, stored) && stored == uid)
                {
                    employeeId = employeeStore.at(i).id;
                    found = true;
                    break;
                }
            }
            _showResult(employeeId, found, 'C');
        }
    }
}

void HomeScreen::_showResult(uint16_t employeeId, bool found, char method)
{
    if (found)
    {
        Employee emp;
        const char *name = _screenManager.getEmployeeStore().findById(employeeId, emp) ? emp.name : "?";

        time_t epoch = _screenManager.getTimeManager().getEpoch();
        _screenManager.getAttendanceLog().append(employeeId, method, epoch);
        _screenManager.getApiService().syncPendingAttendance(_screenManager.getAttendanceLog());

        _resultName = name;
        _resultStatus = "Checked in!";
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, LOW);
        _screenManager.getAudioFeedback().playSuccess();
    }
    else
    {
        _resultName = "";
        _resultStatus = "Not registered";
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        _screenManager.getAudioFeedback().playFailure();
    }

    _idleState = IdleState::RESULT;
    _resultTimer.reset();
    _renderResult();
}
