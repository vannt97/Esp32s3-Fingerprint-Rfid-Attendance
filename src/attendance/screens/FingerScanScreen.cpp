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
    _removeTimerStarted = false;
    _saveStarted = false;
    _savedTemplateId = 0;

    if (!_screenManager.getFingerprintService().isConnected())
    {
        _errorMessage = "Sensor not found";
        _enterState(State::ERROR);
        return;
    }

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
        _displayManager.print(4, 36, "(1/2)");
        break;
    case State::REMOVE_1:
        _displayManager.print(4, 24, "Remove finger...");
        break;
    case State::PLACE_2:
        _displayManager.print(4, 24, "Place finger again");
        _displayManager.print(4, 36, "(2/2)");
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

    if (_state == State::SUCCESS || _state == State::ERROR)
        _displayManager.print(13, 53, "Select");
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

    switch (_state)
    {
    case State::PLACE_1:
        _pollPlace(1, State::REMOVE_1);
        break;
    case State::REMOVE_1:
        _pollRemove();
        break;
    case State::PLACE_2:
        _pollPlace(2, State::SAVING);
        break;
    case State::SAVING:
        if (!_saveStarted)
        {
            _saveStarted = true;
            _doSave();
        }
        break;
    case State::SUCCESS:
    case State::ERROR:
        if (btn == Button::SELECT)
            _screenManager.showScreen(ScreenId::FINGER_SELECT);
        break;
    }
}

void FingerScanScreen::_pollPlace(uint8_t slot, State nextState)
{
    auto &fp = _screenManager.getFingerprintService();
    FingerStepResult result = fp.captureStep();

    if (result == FingerStepResult::NO_FINGER)
        return;

    if (result == FingerStepResult::ERROR)
    {
        _errorMessage = fp.lastErrorString();
        _enterState(State::ERROR);
        return;
    }

    if (!fp.convertImage(slot))
    {
        _errorMessage = fp.lastErrorString();
        _enterState(State::ERROR);
        return;
    }

    _enterState(nextState);
}

void FingerScanScreen::_pollRemove()
{
    auto &fp = _screenManager.getFingerprintService();

    if (!fp.isFingerRemoved())
    {
        _removeTimerStarted = false;
        return;
    }

    if (!_removeTimerStarted)
    {
        _removeTimerStarted = true;
        _removeSettleTimer.reset();
        return;
    }

    if (_removeSettleTimer.isExpired())
    {
        _removeTimerStarted = false;
        _enterState(State::PLACE_2);
    }
}

void FingerScanScreen::_doSave()
{
    auto &fp = _screenManager.getFingerprintService();

    if (!fp.createModel())
    {
        _errorMessage = fp.lastErrorString();
        _enterState(State::ERROR);
        return;
    }

    uint16_t id = fp.allocateNextTemplateId();
    if (id == 0)
    {
        _errorMessage = "Storage full";
        _enterState(State::ERROR);
        return;
    }

    if (!fp.storeModel(id))
    {
        _errorMessage = fp.lastErrorString();
        _enterState(State::ERROR);
        return;
    }

    _savedTemplateId = id;
    _enterState(State::SUCCESS);
}
