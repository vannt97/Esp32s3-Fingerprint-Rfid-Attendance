#pragma once
#include "IScreen.h"
#include "ScreenData.h"
#include "assets/bitmaps.h"
#include "helpers/TimeoutHelper.h"

class ScreenManager; // forward declare, KHÔNG include ScreenManager.h
class HomeScreen : public IScreen
{
public:
    explicit HomeScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class IdleState
    {
        CLOCK,  // đang hiện giờ, âm thầm poll vân tay/thẻ
        RESULT, // đang hiện banner kết quả nhận diện
    };

    void _renderClock();
    void _drawTimeOnly();
    void _renderResult();
    void _pollAttendance();
    bool _pollFingerprint();
    bool _pollCard();
    bool _findEmployeeByCardUid(const String &uid, uint16_t &employeeIdOut);
    void _showResult(uint16_t employeeId, bool found, char method);

    HomeData _data = {
        .time = "--:--",
        .date = "--/--/----",
        .batteryPercent = 85,
        .wifiConnected = false};

    IdleState _idleState = IdleState::CLOCK;
    String _resultName;
    String _resultStatus;

    Timeout _getTimeTimer{GET_TIME_TIMEOUT};
    Timeout _resultTimer{2500};
    Timeout _cardPollTimer{150};

    ScreenManager &_screenManager;
    DisplayManager &_displayManager;
};
