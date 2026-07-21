#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

// Bọc Adafruit_PN532 (I2C, Wire — tách biệt Wire1 dùng cho OLED) theo pin
// trong pins.h. pollCard() dùng timeout NGẮN để không đứng hình vòng
// loop() cooperative của ScreenManager (khác với timeout 1000ms trong code
// test độc lập src/rfid/main.cpp).
class RfidService
{
public:
    RfidService();

    bool begin();
    bool isConnected() const { return _connected; }

    // Poll 1 lần; trả true và điền uidOut (dạng "AA:BB:CC:DD") nếu đọc được thẻ.
    bool pollCard(String &uidOut);

private:
    static constexpr uint16_t POLL_TIMEOUT_MS = 50;

    Adafruit_PN532 _nfc;
    bool _connected = false;
};
