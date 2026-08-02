// display — dùng chung pin map với "pins.h" (OLED trên Wire1, tách khỏi PN532/Wire)
#include "pins.h"

#define SCREEN_WIDTH  OLED_WIDTH
#define SCREEN_HEIGHT OLED_HEIGHT
#define I2C_SDA       OLED_SDA_PIN
#define I2C_SCL       OLED_SCL_PIN
#define OLED_RESET    -1 // Reset chân -1 nếu dùng chung chân reset của ESP
// OLED_ADDR đã định nghĩa trong pins.h


// WiFi
#define TIMEZONE "Asia/Ho_Chi_Minh"
#define NTP_SERVER "pool.ntp.org"
#define NTP_SERVER2 "time.google.com"
#define GMT_OFFSET 7 * 3600 // GMT+7 Việt Nam
#define DAYLIGHT_OFFSET 0
#define WIFI_SSID "LAU 2"
#define WIFI_PASSWORD "12345678"
// #define WIFI_SSID "42/3 UVK - GUEST"
// #define WIFI_PASSWORD "423xinchao"
#define CONNECT_WIFI_TIMEOUT 10000
#define TIME_TIMEOUT 5000
#define GET_TIME_TIMEOUT 1000

// Server Django (đồng bộ danh sách nhân viên + đẩy log chấm công)
// SỬA API_BASE_URL đúng IP LAN của máy đang chạy `docker compose up` trong /server —
// không dùng "localhost" vì ESP32 là thiết bị khác trên mạng.
#define API_BASE_URL "http://localhost:8000"
#define API_DEVICE_TOKEN "c6b064939d0431bb70702b63f18c6e81f4b089db"
#define API_ATTENDANCE_SYNC_INTERVAL_MS (60UL * 1000UL)
#define API_EMPLOYEE_REFETCH_INTERVAL_MS (30UL * 60UL * 1000UL)


// menu
#define MAX_MENU_ITEMS 7
#define INIT_ITEM_POS 3
#define TRAVEL_DISTANCE 6
#define SCROLL_END_POINT  (INIT_ITEM_POS + (TRAVEL_DISTANCE * (MAX_MENU_ITEMS - 1)))
#define SCROLL_THUMB_WIDTH 5
#define SCROLL_THUMB_HEIGHT 17
#define ACTIVE_BORDER_WIDTH 113
#define ACTIVE_BORDER_HEIGHT 24
#define MENU_SCREEN_WIDTH 119
#define MENU_SCREEN_HEIGHT 52
#define ITEM_Y_POS_1 5
#define ITEM_Y_POS_2 29

// board info 
#define LINE_HEIGHT 8