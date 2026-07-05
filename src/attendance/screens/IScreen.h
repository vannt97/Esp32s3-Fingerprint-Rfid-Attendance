#pragma once
#include "../display/DisplayManager.h"

class IScreen {
public:
    
    virtual ~IScreen() {};
    virtual void onEnter() = 0;   
    virtual void onExit() = 0;    
    virtual void loop() = 0;
};