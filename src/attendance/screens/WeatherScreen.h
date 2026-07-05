#pragma once
#include "IScreen.h"
#include "services/WeatherService.h"

class ScreenManager;

class WeatherScreen : public IScreen
{
public:
    explicit WeatherScreen(ScreenManager &sm);
    void onEnter() override;
    void onExit() override;
    void loop() override;

private:
    enum class State { LOADING, SUCCESS, ERROR };

    void _render();
    void _renderLoading();
    void _renderSuccess();
    void _renderError();

    ScreenManager  &_screenManager;
    DisplayManager &_displayManager;
    WeatherService  _service;

    State _state       = State::LOADING;
    int   _selectedIdx = 0;
};