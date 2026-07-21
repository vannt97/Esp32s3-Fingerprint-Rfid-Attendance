#pragma once
#include "IScreen.h"

class ScreenManager;

// Đọc UID thẻ RFID thật qua RfidService (PN532). PLACE/READING polling
// UID mỗi tick loop(), DETECTED hiện UID vừa đọc chờ SELECT xác nhận. Chưa
// có nơi lưu UID ở local/server nên không có bước "Saving" — việc lưu trữ
// thật sẽ làm ở task tích hợp server sau.
class CardScanScreen : public IScreen
{
public:
    explicit CardScanScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class State
    {
        PLACE,
        READING,
        DETECTED,
        SUCCESS,
        ERROR,
    };

    void _enterState(State s);
    void _render();
    void _pollCard();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    State  _state = State::PLACE;
    String _uid;
    String _errorMessage;
};
