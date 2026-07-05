#include "GameScreen.h"
#include "display/ScreenManager.h"

GameScreen::GameScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void GameScreen::onEnter()
{
    _state = State::MENU;
    _menuIdx = 0;
    _renderMenu();
}

void GameScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void GameScreen::loop()
{
    switch (_state)
    {
    case State::MENU:   _loopMenu();    break;
    case State::PLAYING: _loopPlaying(); break;
    }
}

void GameScreen::_renderMenu()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);
    _displayManager.print(0, 0, "Games");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    for (int i = 0; i < GAME_COUNT; i++)
    {
        int y = 16 + i * 12;
        if (i == _menuIdx)
        {
            _displayManager.fillRect(0, y - 1, SCREEN_WIDTH, 11, SSD1306_WHITE);
            _displayManager.setTextColor(SSD1306_BLACK);
        }
        else
        {
            _displayManager.setTextColor(SSD1306_WHITE);
        }
        _displayManager.print(4, y, GAME_NAMES[i]);
    }

    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.print(0, 53, "Play       Exit");
    _displayManager.update();
}

void GameScreen::_loopMenu()
{
    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        if (_menuIdx > 0) { _menuIdx--; _renderMenu(); }
        break;
    case Button::DOWN:
        if (_menuIdx < GAME_COUNT - 1) { _menuIdx++; _renderMenu(); }
        break;
    case Button::SELECT:
        if (_menuIdx == 0) {
            _snake.reset();
            _activeGame = ActiveGame::SNAKE;
        } else {
            _pong.reset();
            _activeGame = ActiveGame::PONG;
        }
        _lastTick = millis();
        _state = State::PLAYING;
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    default:
        break;
    }
}

void GameScreen::_loopPlaying()
{
    if (_activeGame == ActiveGame::PONG)
        _loopPlayingPong();
    else
        _loopPlayingSnake();
}

void GameScreen::_loopPlayingSnake()
{
    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:    _snake.turnUp();    break;
    case Button::DOWN:  _snake.turnDown();  break;
    case Button::LEFT:  _snake.turnLeft();  break;
    case Button::RIGHT: _snake.turnRight(); break;
    case Button::SELECT:
        if (_snake.isGameOver()) _snake.reset();
        break;
    case Button::EXIT:
        _state = State::MENU;
        _renderMenu();
        return;
    default:
        break;
    }

    if (millis() - _lastTick >= SNAKE_TICK_MS)
    {
        _lastTick = millis();
        _snake.update();
        _snake.render(_displayManager);
    }
}

void GameScreen::_loopPlayingPong()
{
    auto &bm = _screenManager.getButtonManager();

    // Single-press: thoat hoac restart
    Button btn = bm.getPressed();
    if (btn == Button::EXIT) {
        _state = State::MENU;
        _renderMenu();
        return;
    }
    if (btn == Button::SELECT && _pong.isGameOver()) {
        _pong.reset();
        _lastTick = millis();
        return;
    }

    if (millis() - _lastTick >= PONG_TICK_MS)
    {
        _lastTick = millis();

        // P1: UP=len, LEFT=xuong | P2: RIGHT=len, DOWN=xuong
        _pong.input(
            bm.isHeld(Button::UP),    // P1 up
            bm.isHeld(Button::LEFT),  // P1 down
            bm.isHeld(Button::RIGHT), // P2 up
            bm.isHeld(Button::DOWN)   // P2 down
        );
        _pong.update();
        _pong.render(_displayManager);
    }
}
