#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

struct CityWeather {
    String  cityName;
    float   temp;
    float   humidity;
    String  description;  // "clear sky", "light rain"
    bool    isValid;
};

struct WeatherCity {
    const char *name;      // tên hiển thị
    const char *cityId;    // OpenWeatherMap city ID
};

class WeatherService {
public:
    WeatherService();

    // Fetch 1 thành phố — blocking, gọi trong task riêng nếu cần
    CityWeather fetch(const WeatherCity &city);

    // Fetch tất cả cities — cập nhật cache
    void fetchAll();

    // Lấy từ cache — không block
    CityWeather getCached(int index) const;
    int         getCityCount() const;

    bool isCacheValid() const;  // cache còn hạn không (15 phút)

    static const WeatherCity CITIES[];
    static const int         CITY_COUNT;

private:
    static constexpr unsigned long CACHE_TTL_MS = 15 * 60 * 1000UL; // 15 phút
    static const char* API_KEY;
    String _removeVietnameseDiacritics(const String &input); // thêm dòng này
    CityWeather   _cache[4];
    unsigned long _lastFetch = 0;
};