#include "SnakeGame.h"

SnakeGame::SnakeGame()
{
    reset();
}

void SnakeGame::reset()
{
    _length   = 3;
    _dir      = Dir::RIGHT;
    _nextDir  = Dir::RIGHT;
    _score    = 0;
    _gameOver = false;

    // Spawn rắn ở giữa màn hình
    int startX = GRID_W / 2;
    int startY = GRID_H / 2;
    for (int i = 0; i < _length; i++)
        _body[i] = {startX - i, startY};

    _spawnFood();
}

void SnakeGame::update()
{
    if (_gameOver) return;

    // Apply input đã buffer
    _dir = _nextDir;

    // Tính head mới
    Point newHead = _body[0];
    switch (_dir)
    {
    case Dir::UP:    newHead.y--; break;
    case Dir::DOWN:  newHead.y++; break;
    case Dir::LEFT:  newHead.x--; break;
    case Dir::RIGHT: newHead.x++; break;
    }

    // Check va tường
    if (newHead.x < 0 || newHead.x >= GRID_W ||
        newHead.y < 0 || newHead.y >= GRID_H)
    {
        _gameOver = true;
        return;
    }

    // Check tự cắn (bỏ qua đuôi vì sẽ move)
    for (int i = 0; i < _length - 1; i++)
    {
        if (_pointEquals(newHead, _body[i]))
        {
            _gameOver = true;
            return;
        }
    }

    // Ăn mồi?
    bool ate = _pointEquals(newHead, _food);

    // Dịch body — từ đuôi lên đầu
    int newLength = ate ? _length + 1 : _length;
    for (int i = min(newLength - 1, MAX_LENGTH - 1); i > 0; i--)
        _body[i] = _body[i - 1];

    _body[0] = newHead;
    _length  = min(newLength, MAX_LENGTH);

    if (ate)
    {
        _score++;
        _spawnFood();
    }
}

void SnakeGame::render(DisplayManager &dm)
{
    dm.clear();
    dm.setTextColor(SSD1306_WHITE);
    dm.setTextWrap(false);

    if (_gameOver)
    {
        dm.print(20, 10, "GAME OVER");
        dm.print(30, 28, "Score: " + String(_score));
        dm.print(10, 46, "[SEL] restart");
        dm.update();
        return;
    }

    // Score bar trên cùng
    dm.print(0, 0, "Score:" + String(_score));
    dm.drawLine(0, 8, 128, 8, SSD1306_WHITE);

    // Vẽ snake — offset y thêm 9px cho score bar
    for (int i = 0; i < _length; i++)
    {
        int px = _body[i].x * CELL_SIZE;
        int py = _body[i].y * CELL_SIZE + 9;

        if (i == 0)
            // Đầu rắn — fill đặc
            dm.fillRect(px, py, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
        else
            // Thân — chừa 1px viền để thấy từng đốt
            dm.fillRect(px + 1, py + 1, CELL_SIZE - 2, CELL_SIZE - 2, SSD1306_WHITE);
    }

    // Vẽ mồi — hình chấm nhỏ 2x2 ở giữa cell
    int fx = _food.x * CELL_SIZE + 1;
    int fy = _food.y * CELL_SIZE + 9 + 1;
    dm.fillRect(fx, fy, 2, 2, SSD1306_WHITE);

    dm.update();
}

bool SnakeGame::isGameOver() const { return _gameOver; }
int  SnakeGame::getScore()   const { return _score; }

void SnakeGame::turnUp()
{
    if (_dir != Dir::DOWN)  _nextDir = Dir::UP;
}
void SnakeGame::turnDown()
{
    if (_dir != Dir::UP)    _nextDir = Dir::DOWN;
}
void SnakeGame::turnLeft()
{
    if (_dir != Dir::RIGHT) _nextDir = Dir::LEFT;
}
void SnakeGame::turnRight()
{
    if (_dir != Dir::LEFT)  _nextDir = Dir::RIGHT;
}

void SnakeGame::_spawnFood()
{
    // Random vị trí không trùng body
    Point candidate;
    bool  conflict;
    do {
        conflict   = false;
        candidate  = {random(GRID_W), random(GRID_H)};
        for (int i = 0; i < _length; i++)
            if (_pointEquals(candidate, _body[i]))
            { conflict = true; break; }
    } while (conflict);

    _food = candidate;
}