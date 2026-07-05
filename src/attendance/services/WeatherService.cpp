#include "WeatherService.h"
#include <ArduinoJson.h>

const char *WeatherService::API_KEY = "54ae5cd87395b5521c083de49df27012";

const WeatherCity WeatherService::CITIES[] = {
    {"Ho Chi Minh", "1566083"},
    {"Ha Noi", "1581130"},
    {"Can Tho", "1586203"},
    {"Nha Trang", "1572151"}};
const int WeatherService::CITY_COUNT = 4;

WeatherService::WeatherService()
{
    // Init cache rỗng
    for (int i = 0; i < CITY_COUNT; i++)
        _cache[i] = {"", 0, 0, "", false};
}

CityWeather WeatherService::fetch(const WeatherCity &city)
{
    CityWeather result = {city.name, 0, 0, "", false};

    if (WiFi.status() != WL_CONNECTED)
        return result;

    HTTPClient http;
    String url = String("http://api.openweathermap.org/data/2.5/weather") + "?id=" + city.cityId + "&appid=" + API_KEY + "&units=metric"; // Celsius
                                                                                                                                          //  + "&lang=vi";                                                                                                           // mô tả tiếng Việt

    http.begin(url);
    int code = http.GET();

    if (code != HTTP_CODE_OK)
    {
        http.end();
        return result;
    }

    String payload = http.getString();

    // Parse JSON
    JsonDocument doc;
    deserializeJson(doc, payload);
    http.end();

    result.temp = doc["main"]["temp"].as<float>();
    result.humidity = doc["main"]["humidity"].as<float>();
    result.description = _removeVietnameseDiacritics(doc["weather"][0]["description"].as<String>());
    result.isValid = true;

    return result;
}

void WeatherService::fetchAll()
{
    for (int i = 0; i < CITY_COUNT; i++)
        _cache[i] = fetch(CITIES[i]);

    _lastFetch = millis();
}

CityWeather WeatherService::getCached(int index) const
{
    if (index < 0 || index >= CITY_COUNT)
        return {"", 0, 0, "", false};
    return _cache[index];
}

int WeatherService::getCityCount() const { return CITY_COUNT; }

bool WeatherService::isCacheValid() const
{
    if (_lastFetch == 0)
        return false;
    return (millis() - _lastFetch) < CACHE_TTL_MS;
}

String WeatherService::_removeVietnameseDiacritics(const String &input)
{
    struct
    {
        const char *from;
        const char *to;
    } map[] = {
        {"mưa nhỏ", "mua nho"},
        {"mưa", "mua"},
        {"nắng", "nang"},
        {"có mây", "co may"},
        {"nhiều mây", "nhieu may"},
        {"quang đãng", "quang dang"},
        {"sương mù", "suong mu"},
        {"giông", "giong"},
        {"tuyết", "tuyet"},
    };

    for (auto &entry : map)
        if (input.indexOf(entry.from) >= 0)
            return String(entry.to);

    return input;
}