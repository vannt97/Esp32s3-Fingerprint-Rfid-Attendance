#pragma once
#include "IGame.h"
#include <Arduino.h>

class SnakeGame : public IGame
{
public:
    SnakeGame();

    void reset()                        override;
    void update()                       override;
    void render(DisplayManager &dm)     override;
    bool isGameOver() const             override;
    int  getScore()   const             override;

    // Input — gọi từ GameScreen
    void turnUp();
    void turnDown();
    void turnLeft();
    void turnRight();

private:
    // Grid 32x16 cell, mỗi cell 4x4 pixel → 128x64 pixel
    static constexpr int CELL_SIZE  = 4;
    static constexpr int GRID_W     = 128 / CELL_SIZE;  // 32
    static constexpr int GRID_H     = 56  / CELL_SIZE;  // 14 (chừa 8px trên cho score)
    static constexpr int SCORE_Y    = 56;                // y bắt đầu vùng score
    static constexpr int MAX_LENGTH = GRID_W * GRID_H;  // tối đa

    enum class Dir { UP, DOWN, LEFT, RIGHT };

    struct Point { int x; int y; };

    bool _pointEquals(Point a, Point b) const { return a.x == b.x && a.y == b.y; }
    void _spawnFood();
    bool _checkCollision() const;  // va tường hoặc tự cắn

    // Snake body — index 0 là đầu
    Point _body[MAX_LENGTH];
    int   _length    = 0;

    Dir   _dir       = Dir::RIGHT;
    Dir   _nextDir   = Dir::RIGHT;  // buffer input, tránh reverse ngay lập tức

    Point _food      = {0, 0};
    int   _score     = 0;
    bool  _gameOver  = false;
};