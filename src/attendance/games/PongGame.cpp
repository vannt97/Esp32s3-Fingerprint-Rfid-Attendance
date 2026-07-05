#include "PongGame.h"
#include <Arduino.h>

PongGame::PongGame() { reset(); }

void PongGame::reset() {
    _p1Y = GAME_TOP + (GAME_H - PADDLE_H) / 2;
    _p2Y = GAME_TOP + (GAME_H - PADDLE_H) / 2;
    _score1 = 0;
    _score2 = 0;
    _gameOver = false;
    _p1Up = _p1Down = _p2Up = _p2Down = false;
    _resetBall(1);
}

void PongGame::_resetBall(int dir) {
    _ballX = (GAME_W - BALL_SIZE) / 2.0f;
    _ballY = GAME_TOP + (GAME_H - BALL_SIZE) / 2.0f;
    _ballVX = SPEED_INIT * (dir > 0 ? 1.0f : -1.0f);
    float vy = 0.5f + (random(10) / 10.0f);
    _ballVY = random(2) ? vy : -vy;
}

void PongGame::input(bool p1Up, bool p1Down, bool p2Up, bool p2Down) {
    _p1Up = p1Up; _p1Down = p1Down;
    _p2Up = p2Up; _p2Down = p2Down;
}

void PongGame::update() {
    if (_gameOver) return;

    // Move paddles
    if (_p1Up)   _p1Y -= PADDLE_SPD;
    if (_p1Down) _p1Y += PADDLE_SPD;
    if (_p2Up)   _p2Y -= PADDLE_SPD;
    if (_p2Down) _p2Y += PADDLE_SPD;

    if (_p1Y < GAME_TOP)             _p1Y = GAME_TOP;
    if (_p1Y + PADDLE_H > GAME_BOT)  _p1Y = GAME_BOT - PADDLE_H;
    if (_p2Y < GAME_TOP)             _p2Y = GAME_TOP;
    if (_p2Y + PADDLE_H > GAME_BOT)  _p2Y = GAME_BOT - PADDLE_H;

    // Move ball
    _ballX += _ballVX;
    _ballY += _ballVY;

    // Top/bottom wall bounce
    if (_ballY < GAME_TOP) {
        _ballY = GAME_TOP;
        _ballVY = fabsf(_ballVY);
    }
    if (_ballY + BALL_SIZE > GAME_BOT) {
        _ballY = GAME_BOT - BALL_SIZE;
        _ballVY = -fabsf(_ballVY);
    }

    // Left paddle collision
    if (_ballVX < 0.0f &&
        _ballX            <= P1_X + PADDLE_W &&
        _ballX + BALL_SIZE >= P1_X &&
        _ballY + BALL_SIZE >= _p1Y &&
        _ballY             <= _p1Y + PADDLE_H)
    {
        _ballX  = P1_X + PADDLE_W;
        _ballVX = fabsf(_ballVX) + SPEED_INC;
        if (_ballVX > SPEED_MAX) _ballVX = SPEED_MAX;
        float rel = (_ballY + BALL_SIZE * 0.5f) - (_p1Y + PADDLE_H * 0.5f);
        _ballVY = rel * 0.2f;
    }

    // Right paddle collision
    if (_ballVX > 0.0f &&
        _ballX + BALL_SIZE >= P2_X &&
        _ballX             <= P2_X + PADDLE_W &&
        _ballY + BALL_SIZE >= _p2Y &&
        _ballY             <= _p2Y + PADDLE_H)
    {
        _ballX  = P2_X - BALL_SIZE;
        _ballVX = -(fabsf(_ballVX) + SPEED_INC);
        if (_ballVX < -SPEED_MAX) _ballVX = -SPEED_MAX;
        float rel = (_ballY + BALL_SIZE * 0.5f) - (_p2Y + PADDLE_H * 0.5f);
        _ballVY = rel * 0.2f;
    }

    // Scoring
    if (_ballX + BALL_SIZE < 0) {
        _score2++;
        if (_score2 >= WIN_SCORE) { _gameOver = true; return; }
        _resetBall(1);
    } else if (_ballX > GAME_W) {
        _score1++;
        if (_score1 >= WIN_SCORE) { _gameOver = true; return; }
        _resetBall(-1);
    }
}

void PongGame::render(DisplayManager &dm) {
    dm.clear();
    dm.setTextSize(1);
    dm.setTextColor(SSD1306_WHITE);

    // Score: P1 ben trai, P2 ben phai
    char buf[4];
    snprintf(buf, sizeof(buf), "%d", _score1);
    dm.print(20, 0, buf);
    snprintf(buf, sizeof(buf), "%d", _score2);
    dm.print(100, 0, buf);

    // Separator
    dm.drawLine(0, GAME_TOP - 1, GAME_W - 1, GAME_TOP - 1, SSD1306_WHITE);

    if (_gameOver) {
        dm.print(16, 24, _score1 >= WIN_SCORE ? "P1 WINS!" : "P2 WINS!");
        dm.print(4, 40, "SELECT:play again");
        dm.update();
        return;
    }

    // Center dashed line
    for (int y = GAME_TOP; y < GAME_BOT; y += 4) {
        dm.fillRect(63, y, 2, 2, SSD1306_WHITE);
    }

    // Paddles
    dm.fillRect(P1_X, _p1Y, PADDLE_W, PADDLE_H, SSD1306_WHITE);
    dm.fillRect(P2_X, _p2Y, PADDLE_W, PADDLE_H, SSD1306_WHITE);

    // Ball
    dm.fillRect((int)_ballX, (int)_ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);

    dm.update();
}

bool PongGame::isGameOver() const { return _gameOver; }
int  PongGame::getScore()   const { return _score1; }
