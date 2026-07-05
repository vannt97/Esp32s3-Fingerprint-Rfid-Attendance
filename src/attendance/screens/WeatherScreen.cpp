#include "WeatherScreen.h"
#include "display/ScreenManager.h"

WeatherScreen::WeatherScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void WeatherScreen::onEnter()
{
    _state = State::LOADING;
    _renderLoading();

    // Chỉ fetch nếu cache hết hạn
    if (!_service.isCacheValid())
        _service.fetchAll();

    _state = _service.getCached(0).isValid ? State::SUCCESS : State::ERROR;
    _render();
}

void WeatherScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void WeatherScreen::loop()
{
    if (_state != State::SUCCESS) return;

    Button btn = _screenManager.getButtonManager().getPressed();
    switch (btn)
    {
    case Button::UP:
        if (_selectedIdx > 0) { _selectedIdx--; _render(); }
        break;
    case Button::DOWN:
        if (_selectedIdx < WeatherService::CITY_COUNT - 1) { _selectedIdx++; _render(); }
        break;
    case Button::SELECT:
        // Force refresh cache
        _state = State::LOADING;
        _renderLoading();
        _service.fetchAll();
        _state = _service.getCached(0).isValid ? State::SUCCESS : State::ERROR;
        _render();
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;
    default:
        break;
    }
}

void WeatherScreen::_render()
{
    switch (_state)
    {
    case State::LOADING: _renderLoading(); break;
    case State::SUCCESS: _renderSuccess(); break;
    case State::ERROR:   _renderError();   break;
    }
}

void WeatherScreen::_renderLoading()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.print(10, 24, "Fetching weather...");
    _displayManager.update();
}

void WeatherScreen::_renderError()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.print(0, 16, "Failed to fetch");
    _displayManager.print(0, 32, "Check WiFi");
    _displayManager.print(0, 53, "[SEL] retry  [EXIT] back");
    _displayManager.update();
}

void WeatherScreen::_renderSuccess()
{
    CityWeather w = _service.getCached(_selectedIdx);

    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    // Header: < 1/4 Ho Chi Minh >
    String counter = String(_selectedIdx + 1) + "/" + String(WeatherService::CITY_COUNT);
    String cityLine = "< " + counter + " " + w.cityName + " >";
    _displayManager.print(0, 0, cityLine);
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    // Nhiệt độ
    _displayManager.setTextSize(2);
    String tempStr = String((int)w.temp) + "C";
    _displayManager.print(10, 18, tempStr);
    _displayManager.setTextSize(1);

    // Chi tiết
    _displayManager.print(60, 18, "Hum: " + String((int)w.humidity) + "%");
    _displayManager.print(0, 40, "Des: " +  w.description);

    // Footer — ẩn mũi tên nếu đầu/cuối danh sách
    String left  = (_selectedIdx > 0)                              ? "<" : " ";
    String right = (_selectedIdx < WeatherService::CITY_COUNT - 1) ? ">" : " ";
    _displayManager.print(2,   53, left);
    _displayManager.print(122, 53, right);
    _displayManager.print(20,   53, "[SEL] refresh");

    _displayManager.update();
}