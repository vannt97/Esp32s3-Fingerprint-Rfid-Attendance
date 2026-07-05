#pragma once
#include "IScreen.h"
#include "games/SnakeGame.h"
#include "games/PongGame.h"
#include <Arduino.h>

class ScreenManager;

class GameScreen : public IScreen
{
public:
    explicit GameScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class State      { MENU, PLAYING };
    enum class ActiveGame { SNAKE, PONG };

    void _renderMenu();
    void _loopMenu();
    void _loopPlaying();
    void _loopPlayingSnake();
    void _loopPlayingPong();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;

    State       _state      = State::MENU;
    ActiveGame  _activeGame = ActiveGame::SNAKE;
    SnakeGame   _snake;
    PongGame    _pong;

    int _menuIdx = 0;
    static constexpr const char* GAME_NAMES[] = {"Snake", "Pong 2P"};
    static constexpr int         GAME_COUNT   = 2;

    unsigned long _lastTick = 0;
    static constexpr unsigned long SNAKE_TICK_MS = 150;
    static constexpr unsigned long PONG_TICK_MS  = 33;
};
