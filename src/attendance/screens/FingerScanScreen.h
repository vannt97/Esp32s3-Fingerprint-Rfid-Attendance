#pragma once
#include "IScreen.h"

class ScreenManager;

// Chỉ dựng UI cho flow quét vân tay 3 lần. _tick() để trống — nối
// FingerprintService thật (getImage/image2Tz/createModel/storeModel) và tự
// gọi _enterState()/_setError() để chuyển bước.
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
        REMOVE_2,
        PLACE_3,
        SAVING,
        SUCCESS,
        ERROR,
    };

    void _enterState(State s);
    void _render();
    void _tick();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    State  _state = State::PLACE_1;
    int    _fingerIndex = 0;
    String _errorMessage;
};
