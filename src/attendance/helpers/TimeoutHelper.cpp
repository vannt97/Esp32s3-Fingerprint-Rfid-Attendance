#include "TimeoutHelper.h"

// Constructor sử dụng Initializer List
Timeout::Timeout(unsigned long timeoutMs) 
    : _timeoutMs(timeoutMs), _elapsedBeforePause(0), _isPaused(false) {
    reset();
}

void Timeout::reset() {
    _startTime = millis();
    _elapsedBeforePause = 0;
    _isPaused = false;
}

void Timeout::pause() {
    if (!_isPaused) {
        _elapsedBeforePause = millis() - _startTime;
        _isPaused = true;
    }
}

void Timeout::resume() {
    if (_isPaused) {
        _startTime = millis() - _elapsedBeforePause;
        _isPaused = false;
    }
}

unsigned long Timeout::elapsed() {
    if (_isPaused) return _elapsedBeforePause;
    return millis() - _startTime;
}

bool Timeout::isRunning() {
    if (_isPaused) return true;
    return (elapsed() < _timeoutMs);
}

bool Timeout::isExpired() {
    if (_isPaused) return false;
    if (elapsed() >= _timeoutMs) {
        reset();
        return true;
    }
    return false;
}

bool Timeout::isPaused() {
    return _isPaused;
}