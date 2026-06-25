#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include "pins.h"

// I2C mode — IRQ và RST không bắt buộc, để -1 nếu không nối
Adafruit_PN532 nfc(-1, -1);

// ---------- helpers ----------

void printUID(uint8_t *uid, uint8_t len) {
    Serial.print(F("UID: "));
    for (uint8_t i = 0; i < len; i++) {
        if (uid[i] < 0x10) Serial.print('0');
        Serial.print(uid[i], HEX);
        if (i < len - 1) Serial.print(':');
    }
    Serial.println();
}

const char* cardTypeName(uint8_t uidLen) {
    switch (uidLen) {
        case 4: return "Mifare Classic 1K/4K";
        case 7: return "Mifare Ultralight / NTAG2xx";
        default: return "Unknown";
    }
}

// ---------- I2C scan tìm địa chỉ ----------

void i2cScan(uint8_t sda, uint8_t scl) {
    Wire.end();
    pinMode(sda, INPUT_PULLUP);
    pinMode(scl, INPUT_PULLUP);
    Wire.begin(sda, scl);
    Wire.setClock(100000);
    delay(50);

    Serial.printf("  I2C scan SDA=GPIO%d SCL=GPIO%d ...\n", sda, scl);
    int found = 0;
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.printf("    [FOUND] 0x%02X", addr);
            if (addr == 0x24) Serial.print(F("  <- PN532"));
            Serial.println();
            found++;
        }
    }
    if (!found) Serial.println(F("    No device found"));
}

// ---------- test routines ----------

bool testConnection() {
    Serial.println(F("[TEST] Connecting to PN532 via I2C..."));

    // Thử SDA=8/SCL=7 và SDA=8/SCL=9
    const uint8_t pairs[][2] = {{8, 7}, {8, 9}, {7, 8}, {9, 8}};
    for (auto &p : pairs) {
        Wire.end();
        pinMode(p[0], INPUT_PULLUP);
        pinMode(p[1], INPUT_PULLUP);
        Wire.begin(p[0], p[1]);
        Wire.setClock(100000);
        delay(50);
        nfc.begin();

        uint32_t ver = nfc.getFirmwareVersion();
        if (ver) {
            Serial.println(F("[PASS] PN532 found!"));
            Serial.printf("  Chip    : PN5%02X\n", (ver >> 24) & 0xFF);
            Serial.printf("  Firmware: %d.%d\n", (ver >> 16) & 0xFF, (ver >> 8) & 0xFF);
            Serial.printf("  Pins    : SDA=GPIO%d SCL=GPIO%d\n\n", p[0], p[1]);
            return true;
        }
    }

    // Không tìm được — scan để xem có device nào trên bus không
    Serial.println(F("[FAIL] PN532 not found. Running I2C scan...\n"));
    i2cScan(8, 7);
    i2cScan(8, 9);
    return false;
}

void testScanCard() {
    uint8_t uid[7] = {0};
    uint8_t uidLen = 0;

    Serial.println(F("[TEST] Waiting for card/tag... (5s)"));
    if (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, 5000)) {
        Serial.println(F("[INFO] No card detected\n"));
        return;
    }

    Serial.println(F("[PASS] Card detected!"));
    printUID(uid, uidLen);
    Serial.printf("  Type    : %s\n", cardTypeName(uidLen));
    Serial.printf("  UID len : %d bytes\n\n", uidLen);
}

// ---------- setup / loop ----------

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println(F("\n========================================"));
    Serial.println(F("  ESP32-S3 + PN532 I2C Test"));
    Serial.println(F("========================================\n"));

    if (!testConnection()) {
        Serial.println(F("\nHalted — fix wiring then reset."));
        while (true) { delay(1000); }
    }

    nfc.SAMConfig();
    Serial.println(F("Commands: [s] scan once | [r] repeat scan"));
}

void loop() {
    if (!Serial.available()) return;

    char cmd = Serial.read();
    switch (cmd) {
        case 's':
            testScanCard();
            break;
        case 'r':
            Serial.println(F("Continuous scan — send any key to stop"));
            while (!Serial.available()) {
                testScanCard();
            }
            Serial.read();
            Serial.println(F("Stopped.\n"));
            break;
        default:
            break;
    }
}
