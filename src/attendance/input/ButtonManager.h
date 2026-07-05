#pragma once
#include <Arduino.h>
#include "config.h"
// ── Tên từng nút ──────────────────────────────
enum class Button
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    EXIT,
    NONE // không có nút nào được nhấn
};
#define BTN_UP_PIN 39
#define BTN_DOWN_PIN 38
#define BTN_LEFT_PIN 1
#define BTN_RIGHT_PIN 2
#define BTN_SELECT_PIN 11
#define BTN_EXIT_PIN 12
// ── Thời gian chống rung ──────────────────────
#define DEBOUNCE_MS 50

class ButtonManager
{
public:
    explicit ButtonManager();

    void begin();
    Button getPressed(); // rising-edge, có debounce
    bool isHeld(Button btn); // raw state, dùng cho Pong paddle

private:
    bool _isPressed(uint8_t pin); // kiểm tra 1 nút có đang nhấn không

    // lưu thời điểm nhấn cuối của từng nút để chống rung
    unsigned long _lastDebounceUp = 0;
    unsigned long _lastDebounceDown = 0;
    unsigned long _lastDebounceLeft   = 0;  // ← thêm
    unsigned long _lastDebounceRight  = 0;  // ← thêm
    unsigned long _lastDebounceSelect = 0;
    unsigned long _lastDebounceExit = 0;

    // ← thêm: lưu trạng thái lần trước
    bool _lastStateUp = false;
    bool _lastStateDown = false;
    bool _lastStateLeft   = false;  // ← thêm
    bool _lastStateRight  = false;  // ← thêm
    bool _lastStateSelect = false;
    bool _lastStateExit = false;
};