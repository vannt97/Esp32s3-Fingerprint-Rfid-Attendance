#pragma once
#include "IScreen.h"
#include "services/WifiScanner.h"

class ScreenManager; // forward declare, KHÔNG include ScreenManager.h

class WifiManagerScreen : public IScreen
{
public:
    explicit WifiManagerScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class State
    {
        SCANNING,
        LIST,
        EMPTY
    };

    void _doScan();
    void _render();
    void _renderScanning();
    void _renderList();
    void _renderEmpty();


    ScreenManager &_screenManager;
    DisplayManager &_displayManager;
    WifiScanner _scanner;

    State _state = State::SCANNING;
    int _selectedIdx = 0;
    int _scrollOffset = 0;

    static constexpr int VISIBLE_ROWS = 4;
    static constexpr int ROW_H = 12;
    static constexpr int LIST_Y = 10;
};