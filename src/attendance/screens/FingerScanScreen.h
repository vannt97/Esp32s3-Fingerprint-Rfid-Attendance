#pragma once
#include "IScreen.h"
#include "helpers/TimeoutHelper.h"

class ScreenManager;

// Quét vân tay thật qua FingerprintService (AS608). Theo đúng giao thức
// enroll của Adafruit_Fingerprint: 2 lần đặt tay (image2Tz(1), image2Tz(2))
// rồi createModel()+storeModel(). Các state PLACE_x/REMOVE_1 được polling
// mỗi tick loop() (không còn chờ nút SELECT), SELECT chỉ dùng để rời màn
// SUCCESS/ERROR, EXIT hủy quét bất kỳ lúc nào.
class FingerScanScreen : public IScreen
{
public:
    explicit FingerScanScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class State
    {
        PLACE_1,
        REMOVE_1,
        PLACE_2,
        SAVING,
        SUCCESS,
        ERROR,
    };

    void _enterState(State s);
    void _render();
    void _pollPlace(uint8_t slot, State nextState);
    void _pollRemove();
    void _doSave();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    State  _state = State::PLACE_1;
    int    _fingerIndex = 0;
    String _errorMessage;
    uint16_t _savedTemplateId = 0;

    bool    _removeTimerStarted = false;
    Timeout _removeSettleTimer{300};

    bool _saveStarted = false;
};
