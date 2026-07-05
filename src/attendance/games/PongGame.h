#pragma once
#include "IGame.h"

class PongGame : public IGame {
public:
    PongGame();

    void reset()                    override;
    void update()                   override;
    void render(DisplayManager &dm) override;
    bool isGameOver() const         override;
    int  getScore()   const         override;

    // P1: UP/LEFT buttons, P2: RIGHT/DOWN buttons
    void input(bool p1Up, bool p1Down, bool p2Up, bool p2Down);

private:
    // Game area: screen y=8..63, x=0..127
    static constexpr int   GAME_TOP   = 8;
    static constexpr int   GAME_BOT   = 64;
    static constexpr int   GAME_H     = GAME_BOT - GAME_TOP;
    static constexpr int   GAME_W     = 128;
    static constexpr int   PADDLE_W   = 3;
    static constexpr int   PADDLE_H   = 16;
    static constexpr int   BALL_SIZE  = 3;
    static constexpr int   P1_X       = 2;
    static constexpr int   P2_X       = GAME_W - 2 - PADDLE_W;
    static constexpr int   PADDLE_SPD = 2;
    static constexpr int   WIN_SCORE  = 7;
    static constexpr float SPEED_INIT = 1.5f;
    static constexpr float SPEED_MAX  = 3.5f;
    static constexpr float SPEED_INC  = 0.15f;

    float _ballX, _ballY;
    float _ballVX, _ballVY;
    int   _p1Y, _p2Y;
    int   _score1, _score2;
    bool  _gameOver;
    bool  _p1Up, _p1Down, _p2Up, _p2Down;

    void _resetBall(int dir); // +1 toward P2, -1 toward P1
};
