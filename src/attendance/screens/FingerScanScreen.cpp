#include "FingerScanScreen.h"
#include "display/ScreenManager.h"
#include "FingerNames.h"

FingerScanScreen::FingerScanScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void FingerScanScreen::onEnter()
{
    _fingerIndex = _screenManager.getSelectedFingerIndex();
    _errorMessage = "";
    _enterState(State::PLACE_1);
}

void FingerScanScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void FingerScanScreen::_enterState(State s)
{
    _state = s;
    _render();
}

void FingerScanScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, FINGER_NAMES[_fingerIndex]);
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    switch (_state)
    {
    case State::PLACE_1:
        _displayManager.print(4, 24, "Place finger");
        _displayManager.print(4, 36, "(1/3)");
        break;
    case State::REMOVE_1:
        _displayManager.print(4, 24, "Remove finger...");
        break;
    case State::PLACE_2:
        _displayManager.print(4, 24, "Place finger again");
        _displayManager.print(4, 36, "(2/3)");
        break;
    case State::REMOVE_2:
        _displayManager.print(4, 24, "Remove finger...");
        break;
    case State::PLACE_3:
        _displayManager.print(4, 24, "Place finger again");
        _displayManager.print(4, 36, "(3/3)");
        break;
    case State::SAVING:
        _displayManager.print(4, 24, "Saving...");
        break;
    case State::SUCCESS:
        _displayManager.print(4, 24, "Success!");
        break;
    case State::ERROR:
        _displayManager.print(4, 24, "Error:");
        _displayManager.print(4, 36, _errorMessage);
        break;
    }

    _displayManager.print(89, 53, "Exit");
    _displayManager.update();
}

void FingerScanScreen::loop()
{
    Button btn = _screenManager.getButtonManager().getPressed();
    if (btn == Button::EXIT)
    {
        _screenManager.showScreen(ScreenId::FINGER_SELECT);
        return;
    }

    _tick();
}

void FingerScanScreen::_tick()
{
    // TODO: nối FingerprintService thật ở đây, mỗi loop() gọi 1 lệnh cảm biến
    // rồi tự _enterState() sang bước kế:
    //   PLACE_1  -> capture + image2Tz(1)         -> REMOVE_1
    //   REMOVE_1 -> chờ nhấc tay (NOFINGER)        -> PLACE_2
    //   PLACE_2  -> capture + image2Tz(2)          -> REMOVE_2
    //   REMOVE_2 -> chờ nhấc tay (NOFINGER)        -> PLACE_3
    //   PLACE_3  -> capture xác nhận lần 3         -> SAVING
    //   SAVING   -> createModel() + storeModel(id) -> SUCCESS / ERROR
    // Lỗi ở bước nào thì set _errorMessage rồi _enterState(State::ERROR).
}
