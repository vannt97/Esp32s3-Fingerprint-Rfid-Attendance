#pragma once

// AS608 Fingerprint Sensor — UART1
#define FP_RX_PIN   18   // ESP32-S3 RX ← AS608 TX
#define FP_TX_PIN   17   // ESP32-S3 TX → AS608 RX
#define FP_BAUD     57600

// PN532 RFID/NFC — UART/HSU mode
// Module TXD → ESP32 RX | Module RXD → ESP32 TX
#define PN532_RX_PIN  16   // ESP32 RX ← Module TXD
#define PN532_TX_PIN  15   // ESP32 TX → Module RXD
#define PN532_BAUD    115200

// Status LEDs (optional)
#define LED_GREEN   5
#define LED_RED     6
