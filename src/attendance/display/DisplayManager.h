#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "config.h"

class DisplayManager
{
public:
    static DisplayManager &getInstance();

    bool begin();
    Adafruit_SSD1306 &getDisplay();

    // Tiện ích vẽ chung
    void clear();
    void update();
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setTextColor(uint16_t c);
    void print(int16_t x, int16_t y, String text);
    void setTextWrap(bool w);
    void setTextSize(uint8_t s);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
    void drawCenteredWithBitmap(
        int16_t y_bitmap, int16_t y_text,
        const uint8_t *bitmap, int16_t bitmapW, int16_t bitmapH,
        const String &text,
        int16_t gap = 7,
        uint16_t color = WHITE);
    // Ngăn copy (Singleton)
    DisplayManager(const DisplayManager &) = delete;
    void operator=(const DisplayManager &) = delete;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

private:
    DisplayManager();
    Adafruit_SSD1306 _display;
    uint8_t _dotCount = 0;
};
