#pragma once
#include <Arduino.h>

class Timeout {
  private:
    unsigned long _startTime;
    unsigned long _timeoutMs;
    unsigned long _elapsedBeforePause;
    bool _isPaused;

  public:
    Timeout(unsigned long timeoutMs); // Chỉ khai báo constructor
    void reset();                     // Chỉ khai báo hàm
    void pause();
    void resume();
    unsigned long elapsed();
    bool isRunning();
    bool isExpired();
    bool isPaused();
};