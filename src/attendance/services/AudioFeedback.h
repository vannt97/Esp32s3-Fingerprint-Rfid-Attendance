#pragma once
#include <Arduino.h>
#include <Audio.h>

// Phát âm thanh xác nhận chấm công qua loa MAX98357A (I2S), theo đúng
// pattern đã chạy được ở src/audio/main.cpp. loop() PHẢI được gọi mỗi
// tick main loop để thư viện tiếp tục giải mã/phát (không tự chạy nền).
class AudioFeedback
{
public:
    bool begin();
    void loop();

    void playSuccess(); // /xacthuc.mp3
    void playFailure(); // /xac-thuc-khong-thanh-cong.mp3

private:
    Audio _audio;
};