#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "pins.h"

static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire1, -1);

// Hiển thị 2 dòng: line1 lớn (textSize 2), line2 nhỏ (textSize 1)
static void show(const char *line1, const char *line2 = nullptr)
{
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    oled.setTextSize(2);
    int x1 = (OLED_WIDTH - (int)(strlen(line1) * 12)) / 2;
    oled.setCursor(x1 < 0 ? 0 : x1, line2 ? 12 : 24);
    oled.print(line1);

    if (line2)
    {
        oled.setTextSize(1);
        int x2 = (OLED_WIDTH - (int)(strlen(line2) * 6)) / 2;
        oled.setCursor(x2 < 0 ? 0 : x2, 44);
        oled.print(line2);
    }

    oled.display();
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Wire1.begin(OLED_SDA_PIN, OLED_SCL_PIN);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println(F("[OLED] SSD1306 not found"));
        Serial.printf("  SDA=GPIO%d  SCL=GPIO%d  ADDR=0x%02X\n",
                      OLED_SDA_PIN, OLED_SCL_PIN, OLED_ADDR);
        while (true)
            delay(1000);
    }

    Serial.println(F("[OLED] SSD1306 OK"));

    // ── Sequence test ──────────────────────────────────────────────────────

    // 1. Màn hình trắng toàn bộ (kiểm tra pixel chết)
    oled.fillScreen(SSD1306_WHITE);
    oled.display();
    delay(800);

    // 2. Connecting...
    show("Connecting", "...");
    Serial.println(F("[OLED] Showing: Connecting..."));
    delay(2500);

    // 3. SYNCED
    show("SYNCED", "All sensors ready");
    Serial.println(F("[OLED] Showing: SYNCED"));
    delay(2500);

    // 4. Thông tin pin
    char pinInfo[22];
    snprintf(pinInfo, sizeof(pinInfo), "SDA=%d SCL=%d", OLED_SDA_PIN, OLED_SCL_PIN);
    show("OLED OK", pinInfo);
    Serial.println(F("[OLED] Test complete"));
}

void loop()
{
    // Nhấp nháy "SYNCED" mỗi 3 giây để xác nhận loop hoạt động
    show("SYNCED");
    oled.display();
}
