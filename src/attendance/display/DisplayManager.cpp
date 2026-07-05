#include "DisplayManager.h"

DisplayManager::DisplayManager()
    : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET)
{
}

// ── Singleton instance ────────────────────────
DisplayManager &DisplayManager::getInstance()
{
    static DisplayManager instance;
    return instance;
}

// ── Khởi động OLED ───────────────────────────
bool DisplayManager::begin()
{
    // Wire1 riêng cho OLED, tách khỏi Wire (PN532 trên GPIO8/9)
    Wire1.begin(I2C_SDA, I2C_SCL);

    if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        return false;
    }
    _display.setTextColor(WHITE);
    _display.clearDisplay();
    _display.display();
    return true;
}

Adafruit_SSD1306 &DisplayManager::getDisplay()
{
    return _display;
}

// ── Xóa màn hình ─────────────────────────────
void DisplayManager::clear()
{
    _display.clearDisplay();
}

// ── Đẩy buffer lên màn hình ──────────────────
void DisplayManager::update()
{
    _display.display();
}

void DisplayManager::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
    _display.drawBitmap(x, y, bitmap, w, h, color);
}

void DisplayManager::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    _display.drawRect(x, y, w, h, color);
}

void DisplayManager::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    _display.fillRect(x, y, w, h, color);
}

void DisplayManager::setTextColor(uint16_t c)
{
    _display.setTextColor(c);
}

void DisplayManager::print(int16_t x, int16_t y, String text)
{
    _display.setCursor(x, y);
    _display.print(text);
}

void DisplayManager::setTextWrap(bool w)
{
    _display.setTextWrap(w);
}

void DisplayManager::setTextSize(uint8_t s)
{
    _display.setTextSize(s);
}

void DisplayManager::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    _display.drawRoundRect(x, y, w, h, r, color);
}

void DisplayManager::drawCenteredWithBitmap(
    int16_t y_bitmap, int16_t y_text,
    const uint8_t *bitmap, int16_t bitmapW, int16_t bitmapH,
    const String& text,
    int16_t gap,
    uint16_t color)
{
    // Đo chính xác chiều rộng text theo textSize hiện tại
    int16_t tx, ty;
    uint16_t textW, textH;
    _display.getTextBounds(text, 0, 0, &tx, &ty, &textW, &textH);

    int16_t totalW = bitmapW + gap + textW;
    int16_t x = (SCREEN_WIDTH - SCROLL_THUMB_WIDTH - totalW) / 2;

    _display.drawBitmap(x, y_bitmap, bitmap, bitmapW, bitmapH, color);
    _display.setCursor(x + bitmapW + gap, y_text);
    _display.print(text);
}

void DisplayManager::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    _display.drawLine(x0, y0, x1, y1, color);
}