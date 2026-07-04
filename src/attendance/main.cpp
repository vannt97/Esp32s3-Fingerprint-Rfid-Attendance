#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Fingerprint.h>
#include <Adafruit_PN532.h>
#include <Adafruit_SSD1306.h>
#include "pins.h"

// ── OLED (Wire1 — độc lập với PN532) ─────────────────────────────────────

static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire1, -1);
static SemaphoreHandle_t sOledMutex;

static void oledPrint(const char *line1, const char *line2 = nullptr)
{
    xSemaphoreTake(sOledMutex, portMAX_DELAY);
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    // line1 — lớn (textSize 2: 12×16 px/char)
    oled.setTextSize(2);
    int16_t x1 = (OLED_WIDTH - (int16_t)(strlen(line1) * 12)) / 2;
    oled.setCursor(x1 < 0 ? 0 : x1, line2 ? 12 : 24);
    oled.print(line1);

    // line2 — nhỏ (textSize 1: 6×8 px/char)
    if (line2) {
        oled.setTextSize(1);
        int16_t x2 = (OLED_WIDTH - (int16_t)(strlen(line2) * 6)) / 2;
        oled.setCursor(x2 < 0 ? 0 : x2, 44);
        oled.print(line2);
    }

    oled.display();
    xSemaphoreGive(sOledMutex);
}

// ── shared ────────────────────────────────────────────────────────────────

static SemaphoreHandle_t sMutex;
static volatile uint8_t  sReadyFlags = 0;

#define FLAG_FP   0x01
#define FLAG_RFID 0x02

static void slog(const char *fmt, ...)
{
    char buf[128];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    xSemaphoreTake(sMutex, portMAX_DELAY);
    Serial.print(buf);
    xSemaphoreGive(sMutex);
}

// Gọi sau khi mỗi sensor khởi động xong — chỉ show SYNCED khi cả 2 sẵn sàng
static void markReady(uint8_t flag)
{
    xSemaphoreTake(sMutex, portMAX_DELAY);
    uint8_t prev = sReadyFlags;
    sReadyFlags |= flag;
    uint8_t now  = sReadyFlags;
    xSemaphoreGive(sMutex);

    if (now == (FLAG_FP | FLAG_RFID) && prev != now)
        oledPrint("SYNCED", "All sensors ready");
}

// ── fingerprint task (Core 1) ──────────────────────────────────────────────

static HardwareSerial fpSerial(1);
static Adafruit_Fingerprint finger(&fpSerial);

static void fpTask(void *)
{
    fpSerial.begin(FP_BAUD, SERIAL_8N1, FP_RX_PIN, FP_TX_PIN);
    finger.begin(FP_BAUD);
    vTaskDelay(pdMS_TO_TICKS(300));

    if (!finger.verifyPassword())
    {
        slog("[FP] AS608 not found — task stopped\n");
        vTaskDelete(nullptr);
    }
    finger.getParameters();
    slog("[FP] AS608 ready — %d templates stored\n", finger.templateCount);
    markReady(FLAG_FP);

    for (;;)
    {
        uint8_t r = finger.getImage();
        if (r == FINGERPRINT_OK)
        {
            r = finger.image2Tz();
            if (r == FINGERPRINT_OK)
            {
                r = finger.fingerSearch();
                if (r == FINGERPRINT_OK)
                    slog("[FP] MATCH   ID:%-3d  Conf:%d\n", finger.fingerID, finger.confidence);
                else if (r == FINGERPRINT_NOTFOUND)
                    slog("[FP] No match\n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// ── RFID task (Core 0) ────────────────────────────────────────────────────

static Adafruit_PN532 nfc(-1, -1); // I2C trên Wire (pins 8/9)

static void rfidTask(void *)
{
    Wire.begin(PN532_SDA_PIN, PN532_SCL_PIN);
    Wire.setClock(100000);
    nfc.begin();
    vTaskDelay(pdMS_TO_TICKS(300));

    uint32_t ver = nfc.getFirmwareVersion();
    if (!ver)
    {
        slog("[RFID] PN532 not found — task stopped\n");
        vTaskDelete(nullptr);
    }
    slog("[RFID] PN532 FW %d.%d ready\n", (ver >> 16) & 0xFF, (ver >> 8) & 0xFF);
    nfc.SAMConfig();
    markReady(FLAG_RFID);

    for (;;)
    {
        uint8_t uid[7]  = {};
        uint8_t uidLen  = 0;

        if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, 100))
        {
            char buf[24];
            int n = 0;
            for (uint8_t i = 0; i < uidLen; i++)
                n += snprintf(buf + n, sizeof(buf) - n, i ? ":%02X" : "%02X", uid[i]);
            slog("[RFID] UID: %s\n", buf);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

// ── setup / loop ──────────────────────────────────────────────────────────

void setup()
{
    Serial.begin(115200);
    delay(1500);

    sMutex     = xSemaphoreCreateMutex();
    sOledMutex = xSemaphoreCreateMutex();

    // OLED trên Wire1 — khởi động trước khi tạo task
    Wire1.begin(OLED_SDA_PIN, OLED_SCL_PIN);
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
        Serial.println(F("[OLED] SSD1306 not found — check GPIO 21/22 & address"));
    else
        oledPrint("Connecting", "...");

    //                         name    stack   arg      prio  handle  core
    xTaskCreatePinnedToCore(fpTask,   "FP",   8192, nullptr, 2, nullptr, 1);
    xTaskCreatePinnedToCore(rfidTask, "RFID", 8192, nullptr, 2, nullptr, 0);
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
