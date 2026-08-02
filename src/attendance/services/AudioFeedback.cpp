#include "AudioFeedback.h"
#include <LittleFS.h>
#include "pins.h"

// Thư viện ESP32-audioI2S gọi hàm này (nếu có định nghĩa) để báo log/lỗi
// chi tiết lúc mở/giải mã file — không có hàm này thì mọi lỗi bị nuốt âm
// thầm, không thấy gì trên Serial dù phát thất bại.
void audio_info(const char *info)
{
    Serial.print("[Audio] ");
    Serial.println(info);
}

bool AudioFeedback::begin()
{
    _audio.setPinout(I2S_BCLK_PIN, I2S_LRC_PIN, I2S_DOUT_PIN);
    _audio.setVolume(21); // 0-21, max phần mềm — nếu vẫn nhỏ thì do phần cứng (chân GAIN của MAX98357A hoặc loa)
    return true;
}

void AudioFeedback::loop()
{
    _audio.loop();
}

void AudioFeedback::playSuccess()
{
    _audio.connecttoFS(LittleFS, "/xacthuc.mp3");
}

void AudioFeedback::playFailure()
{
    // Tên file phải ngắn — LittleFS/SPIFFS trên ESP32 giới hạn ~31-32 ký
    // tự cả đường dẫn, tên dài hơn sẽ "Failed to open file for reading".
    _audio.connecttoFS(LittleFS, "/xacthuc_loi.mp3");
}