#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "pins.h"

HardwareSerial fpSerial(1);
Adafruit_Fingerprint finger(&fpSerial);

// ---------- helpers ----------

void printSensorInfo() {
    Serial.println(F("\n=== AS608 Sensor Info ==="));
    Serial.printf("Capacity     : %d templates\n", finger.capacity);
    Serial.printf("Security lvl : %d\n", finger.security_level);
    Serial.printf("Stored count : %d\n", finger.templateCount);
    Serial.printf("Baud rate    : %d\n", finger.baud_rate);
    Serial.println(F("=========================\n"));
}

const char* errString(uint8_t code) {
    switch (code) {
        case FINGERPRINT_OK:          return "OK";
        case FINGERPRINT_NOTFOUND:    return "Not found";
        case FINGERPRINT_PACKETRECIEVEERR: return "Packet receive error (check wiring)";
        case FINGERPRINT_NOFINGER:    return "No finger detected";
        case FINGERPRINT_IMAGEFAIL:   return "Imaging failed";
        case FINGERPRINT_IMAGEMESS:   return "Image too messy";
        case FINGERPRINT_FEATUREFAIL: return "Feature extraction failed";
        case FINGERPRINT_NOMATCH:     return "No match";
        case FINGERPRINT_ENROLLMISMATCH: return "Fingerprints did not match";
        default:                      return "Unknown error";
    }
}

// ---------- test routines ----------

struct PinCombo { uint8_t rx; uint8_t tx; };

// Try every combination of pins × baud rates until sensor responds
bool autoDetect(uint8_t &foundRx, uint8_t &foundTx, uint32_t &foundBaud) {
    const PinCombo pins[]  = { {17, 18}, {18, 17} };
    const uint32_t bauds[] = { 57600, 9600, 115200 };

    for (auto &p : pins) {
        for (auto b : bauds) {
            Serial.printf("  trying RX=%d TX=%d baud=%u ... ", p.rx, p.tx, b);
            fpSerial.end();
            fpSerial.begin(b, SERIAL_8N1, p.rx, p.tx);
            finger.begin(b);
            delay(100);
            if (finger.verifyPassword()) {
                Serial.println(F("FOUND!"));
                foundRx   = p.rx;
                foundTx   = p.tx;
                foundBaud = b;
                return true;
            }
            Serial.println(F("no response"));
        }
    }
    return false;
}

bool testConnection() {
    Serial.println(F("[TEST] Checking sensor connection..."));
    if (finger.verifyPassword()) {
        Serial.println(F("[PASS] AS608 responded on default pins"));
        return true;
    }

    Serial.println(F("[INFO] Default pins failed — running auto-detect..."));
    uint8_t rx, tx;
    uint32_t baud;
    if (autoDetect(rx, tx, baud)) {
        Serial.println(F("[PASS] AS608 found!"));
        Serial.println(F(""));
        Serial.println(F("  Fix your wiring or update pins.h:"));
        Serial.printf( "    #define FP_RX_PIN  %d\n", rx);
        Serial.printf( "    #define FP_TX_PIN  %d\n", tx);
        Serial.printf( "    #define FP_BAUD    %u\n", baud);
        Serial.println(F(""));
        return true;
    }

    Serial.println(F("[FAIL] AS608 not found on any pin/baud combination"));
    Serial.println(F("  Check: VCC=3.3V, GND connected, cables not loose"));
    return false;
}

void testGetParams() {
    Serial.println(F("[TEST] Reading sensor parameters..."));
    if (finger.getParameters() == FINGERPRINT_OK) {
        Serial.println(F("[PASS] Parameters read OK"));
        printSensorInfo();
    } else {
        Serial.println(F("[FAIL] Could not read parameters"));
    }
}

void testGetTemplateCount() {
    Serial.println(F("[TEST] Reading stored template count..."));
    if (finger.getTemplateCount() == FINGERPRINT_OK) {
        Serial.printf("[PASS] Templates stored: %d / %d\n\n", finger.templateCount, finger.capacity);
    } else {
        Serial.println(F("[FAIL] Could not read template count"));
    }
}

// Single scan: capture image → convert → search
void testScanOnce() {
    Serial.println(F("[TEST] Place finger on sensor..."));

    // Wait up to 5 s for a finger
    uint32_t deadline = millis() + 5000;
    uint8_t result = FINGERPRINT_NOFINGER;

    while (result == FINGERPRINT_NOFINGER && millis() < deadline) {
        result = finger.getImage();
        delay(50);
    }

    if (result != FINGERPRINT_OK) {
        Serial.printf("[INFO] %s\n", errString(result));
        return;
    }
    Serial.println(F("[PASS] Image captured"));

    result = finger.image2Tz();
    if (result != FINGERPRINT_OK) {
        Serial.printf("[FAIL] Feature extraction: %s\n", errString(result));
        return;
    }
    Serial.println(F("[PASS] Features extracted"));

    result = finger.fingerSearch();
    if (result == FINGERPRINT_OK) {
        Serial.printf("[PASS] Match found — ID: %d  Confidence: %d\n\n",
                      finger.fingerID, finger.confidence);
    } else if (result == FINGERPRINT_NOTFOUND) {
        Serial.println(F("[INFO] No matching template found in database\n"));
    } else {
        Serial.printf("[FAIL] Search error: %s\n\n", errString(result));
    }
}

// Enroll a new fingerprint into the given ID slot (1–127)
bool enrollFingerprint(uint8_t id) {
    Serial.printf("\n[ENROLL] ID slot: %d\n", id);

    // --- Lần 1 ---
    Serial.println(F("  Buoc 1: Dat ngon tay len cam bien..."));
    uint8_t result = FINGERPRINT_NOFINGER;
    while (result != FINGERPRINT_OK) {
        result = finger.getImage();
        if (result == FINGERPRINT_NOFINGER) { delay(50); continue; }
        if (result != FINGERPRINT_OK) {
            Serial.printf("  [ERR] %s\n", errString(result));
            return false;
        }
    }
    Serial.println(F("  [OK] Anh 1 chup xong"));

    result = finger.image2Tz(1);
    if (result != FINGERPRINT_OK) {
        Serial.printf("  [ERR] Feature 1: %s\n", errString(result));
        return false;
    }

    // --- Nhấc tay ---
    Serial.println(F("  Nhac ngon tay ra..."));
    while (finger.getImage() != FINGERPRINT_NOFINGER) delay(50);
    delay(300);

    // --- Lần 2 ---
    Serial.println(F("  Buoc 2: Dat lai ngon tay..."));
    result = FINGERPRINT_NOFINGER;
    while (result != FINGERPRINT_OK) {
        result = finger.getImage();
        if (result == FINGERPRINT_NOFINGER) { delay(50); continue; }
        if (result != FINGERPRINT_OK) {
            Serial.printf("  [ERR] %s\n", errString(result));
            return false;
        }
    }
    Serial.println(F("  [OK] Anh 2 chup xong"));

    result = finger.image2Tz(2);
    if (result != FINGERPRINT_OK) {
        Serial.printf("  [ERR] Feature 2: %s\n", errString(result));
        return false;
    }

    // --- Tạo và lưu template ---
    result = finger.createModel();
    if (result != FINGERPRINT_OK) {
        Serial.printf("  [ERR] Create model: %s\n", errString(result));
        return false;
    }

    result = finger.storeModel(id);
    if (result != FINGERPRINT_OK) {
        Serial.printf("  [ERR] Store model: %s\n", errString(result));
        return false;
    }

    Serial.printf("  [PASS] Van tay da luu vao ID %d\n\n", id);
    return true;
}

// Continuous scan — prints FINGERPRINT OK when a finger is detected
void scanFingerprint() {
    uint8_t result = finger.getImage();
    if (result != FINGERPRINT_OK) return;

    result = finger.image2Tz();
    if (result != FINGERPRINT_OK) return;

    result = finger.fingerSearch();
    if (result == FINGERPRINT_OK) {
        Serial.println(F("FINGERPRINT OK"));
    }
}

// ---------- setup / loop ----------

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println(F("\n========================================"));
    Serial.println(F("  ESP32-S3 + AS608 Fingerprint Test"));
    Serial.println(F("========================================"));

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);

    fpSerial.begin(FP_BAUD, SERIAL_8N1, FP_RX_PIN, FP_TX_PIN);
    finger.begin(FP_BAUD);
    delay(100);

    if (!testConnection()) {
        digitalWrite(LED_RED, HIGH);
        Serial.println(F("\nHalted — fix wiring then reset."));
        while (true) { delay(1000); }
    }

    digitalWrite(LED_GREEN, HIGH);
    testGetParams();
    testGetTemplateCount();

    Serial.println(F("Ready — đưa ngón tay vào để nhận diện..."));
    Serial.println(F("Commands: [e<id>] enroll (vd: e1) | [s] scan once | [i] info"));
}

void loop() {
    scanFingerprint();

    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'e': {
                // Đọc số ID tiếp theo, vd: gõ "e1" → enroll vào slot 1
                uint32_t deadline = millis() + 2000;
                while (!Serial.available() && millis() < deadline) delay(10);
                uint8_t id = Serial.available() ? Serial.parseInt() : 1;
                if (id < 1 || id > 127) {
                    Serial.println(F("[ERR] ID phai tu 1 den 127"));
                } else {
                    enrollFingerprint(id);
                    testGetTemplateCount();
                }
                break;
            }
            case 's':
                testScanOnce();
                break;
            case 'i':
                testGetParams();
                testGetTemplateCount();
                break;
            default:
                break;
        }
    }

    delay(50);
}
