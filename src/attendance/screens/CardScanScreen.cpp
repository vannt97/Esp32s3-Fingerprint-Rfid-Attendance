#include "CardScanScreen.h"
#include "display/ScreenManager.h"

CardScanScreen::CardScanScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void CardScanScreen::onEnter()
{
    _uid = "";
    _errorMessage = "";

    if (!_screenManager.getRfidService().isConnected())
    {
        _errorMessage = "Reader not found";
        _enterState(State::ERROR);
        return;
    }

    _enterState(State::PLACE);
}

void CardScanScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void CardScanScreen::_enterState(State s)
{
    _state = s;
    _render();
}

void CardScanScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Card Number");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    switch (_state)
    {
    case State::PLACE:
        _displayManager.print(4, 24, "Place card near");
        _displayManager.print(4, 36, "reader");
        break;
    case State::READING:
        _displayManager.print(4, 24, "Reading...");
        break;
    case State::DETECTED:
        _displayManager.print(4, 24, "Card detected:");
        _displayManager.print(4, 36, _uid);
        break;
    case State::SUCCESS:
        _displayManager.print(4, 24, "Success!");
        break;
    case State::ERROR:
        _displayManager.print(4, 24, "Error:");
        _displayManager.print(4, 36, _errorMessage);
        break;
    }

    if (_state == State::DETECTED || _state == State::SUCCESS || _state == State::ERROR)
        _displayManager.print(13, 53, "Select");
    _displayManager.print(89, 53, "Exit");
    _displayManager.update();
}

void CardScanScreen::loop()
{
    Button btn = _screenManager.getButtonManager().getPressed();

    if (btn == Button::EXIT)
    {
        _screenManager.showScreen(ScreenId::EMPLOYEE_SELECT);
        return;
    }

    switch (_state)
    {
    case State::PLACE:
        _enterState(State::READING);
        break;
    case State::READING:
        _pollCard();
        break;
    case State::DETECTED:
        if (btn == Button::SELECT)
        {
            _screenManager.getEnrollmentStore().saveCardMapping(
                _screenManager.getSelectedEmployeeId(), _uid);
            _enterState(State::SUCCESS);
        }
        break;
    case State::SUCCESS:
    case State::ERROR:
        if (btn == Button::SELECT)
            _screenManager.showScreen(ScreenId::EMPLOYEE_SELECT);
        break;
    }
}

void CardScanScreen::_pollCard()
{
    String uid;
    if (_screenManager.getRfidService().pollCard(uid))
    {
        _uid = uid;
        _enterState(State::DETECTED);
    }
}
