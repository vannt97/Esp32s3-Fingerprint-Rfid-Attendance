#include "ButtonManager.h"

ButtonManager::ButtonManager() {}

void ButtonManager::begin()
{
    pinMode(BTN_UP_PIN, INPUT_PULLUP);
    pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
    pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
    pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
    pinMode(BTN_SELECT_PIN, INPUT_PULLUP);
    pinMode(BTN_EXIT_PIN, INPUT_PULLUP);
}

bool ButtonManager::_isPressed(uint8_t pin)
{
    return digitalRead(pin) == LOW;
}

Button ButtonManager::getPressed()
{
    unsigned long now = millis();

    // UP
    bool upNow = _isPressed(BTN_UP_PIN);
    if (upNow && !_lastStateUp && (now - _lastDebounceUp > DEBOUNCE_MS))
    {
        _lastDebounceUp = now;
        _lastStateUp = upNow;
        Serial.println("[BTN] UP pressed");
        return Button::UP;
    }
    _lastStateUp = upNow;

    // DOWN
    bool downNow = _isPressed(BTN_DOWN_PIN);
    if (downNow && !_lastStateDown && (now - _lastDebounceDown > DEBOUNCE_MS))
    {
        _lastDebounceDown = now;
        _lastStateDown = downNow;
        Serial.println("[BTN] DOWN pressed");
        return Button::DOWN;
    }
    _lastStateDown = downNow;

    // LEFT
    bool leftNow = _isPressed(BTN_LEFT_PIN);
    if (leftNow && !_lastStateLeft && (now - _lastDebounceLeft > DEBOUNCE_MS))
    {
        _lastDebounceLeft = now;
        _lastStateLeft = leftNow;
        Serial.println("[BTN] LEFT pressed");
        return Button::LEFT;
    }
    _lastStateLeft = leftNow;

    // RIGHT
    bool rightNow = _isPressed(BTN_RIGHT_PIN);
    if (rightNow && !_lastStateRight && (now - _lastDebounceRight > DEBOUNCE_MS))
    {
        _lastDebounceRight = now;
        _lastStateRight = rightNow;
        Serial.println("[BTN] RIGHT pressed");
        return Button::RIGHT;
    }
    _lastStateRight = rightNow;

    // SELECT
    bool selectNow = _isPressed(BTN_SELECT_PIN);
    if (selectNow && !_lastStateSelect && (now - _lastDebounceSelect > DEBOUNCE_MS))
    {
        _lastDebounceSelect = now;
        _lastStateSelect = selectNow;
        Serial.println("[BTN] SELECT pressed");
        return Button::SELECT;
    }
    _lastStateSelect = selectNow;

    // EXIT
    bool exitNow = _isPressed(BTN_EXIT_PIN);
    if (exitNow && !_lastStateExit && (now - _lastDebounceExit > DEBOUNCE_MS))
    {
        _lastDebounceExit = now;
        _lastStateExit = exitNow;
        Serial.println("[BTN] EXIT pressed");
        return Button::EXIT;
    }
    _lastStateExit = exitNow;

    return Button::NONE;
}

bool ButtonManager::isHeld(Button btn)
{
    switch (btn)
    {
    case Button::UP:
        return digitalRead(BTN_UP_PIN) == LOW;
    case Button::DOWN:
        return digitalRead(BTN_DOWN_PIN) == LOW;
    case Button::LEFT:
        return digitalRead(BTN_LEFT_PIN) == LOW;
    case Button::RIGHT:
        return digitalRead(BTN_RIGHT_PIN) == LOW;
    case Button::SELECT:
        return digitalRead(BTN_SELECT_PIN) == LOW;
    case Button::EXIT:
        return digitalRead(BTN_EXIT_PIN) == LOW;
    default:
        return false;
    }
}