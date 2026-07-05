#pragma once
#include "display/DisplayManager.h"
#include "input/ButtonManager.h"

class IGame {
public:
    virtual ~IGame() = default;
    virtual void reset()               = 0;
    virtual void update()              = 0;
    virtual void render(DisplayManager &dm) = 0;
    virtual bool isGameOver() const    = 0;
    virtual int  getScore() const      = 0;
};