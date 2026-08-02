#pragma once

// AS608 Fingerprint Sensor — UART1
#define FP_RX_PIN   18   // ESP32-S3 RX ← AS608 TX
#define FP_TX_PIN   17   // ESP32-S3 TX → AS608 RX
#define FP_BAUD     57600

// PN532 RFID/NFC — I2C mode (điều chỉnh nếu test_rfid tìm ra cặp pin khác)
#define PN532_SDA_PIN  8
#define PN532_SCL_PIN  9

// Status LEDs (optional) — đổi khỏi GPIO 5/6 vì trùng chân I2S của loa
// MAX98357A bên dưới (xem ghi chú ở đó). Sửa lại 2 số này nếu bạn đã nối
// LED vào chân khác trên board thật.
#define LED_GREEN   15
#define LED_RED     16

// SSD1306 OLED — Wire1 (độc lập với PN532 trên Wire)
// ESP32-S3 không có GPIO 22/23; GPIO 19/20 = USB; GPIO 48 = RGB LED
#define OLED_SDA_PIN  21
#define OLED_SCL_PIN  47
#define OLED_ADDR     0x3C
#define OLED_WIDTH    128
#define OLED_HEIGHT   64

// MAX98357A I2S Amp — dùng cho env:test_audio và env:attendance
// (AudioFeedback). Trước đây GPIO 5/6 trùng LED_GREEN/LED_RED — đã đổi
// LED sang 15/16 ở trên để hết xung đột.
#define I2S_BCLK_PIN  6   // MAX98357A BCLK
#define I2S_LRC_PIN   7   // MAX98357A LRC (WS)
#define I2S_DOUT_PIN  5   // MAX98357A DIN
// SD_MODE của MAX98357A: nối thẳng lên 3.3V để luôn bật (không dùng GPIO)
