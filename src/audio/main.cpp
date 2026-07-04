#include <Arduino.h>
#include <Audio.h>
#include <LittleFS.h>

// Chân I2S cho MAX98357A
#define I2S_BCLK  6
#define I2S_LRC   7
#define I2S_DOUT  5

Audio audio;

unsigned long lastPlay = 0;
const unsigned long interval = 10000; // 3 giây

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("Loi mount LittleFS");
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(18); // 0-21
}

void loop() {
  audio.loop();

  if (millis() - lastPlay >= interval) {
    lastPlay = millis();
    Serial.println("Phat am thanh...");
    audio.connecttoFS(LittleFS, "/xacthuc.mp3");
  }
}