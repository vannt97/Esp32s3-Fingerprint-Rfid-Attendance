Hiển thị sơ đồ kết nối pin cho project ESP32-S3 + AS608 + RFID.

Đọc file platformio.ini và các file header trong src/ hoặc include/ để tìm định nghĩa pin (#define, const int).
Sau đó hiển thị bảng pinout thực tế của project này.

Nếu chưa có định nghĩa pin trong code, hiển thị bảng gợi ý mặc định hợp lý:

## Pinout gợi ý cho ESP32-S3

### AS608 Fingerprint Sensor
| AS608 | ESP32-S3 | Ghi chú |
|-------|----------|---------|
| VCC   | 3.3V     | KHÔNG dùng 5V |
| GND   | GND      | |
| TX    | GPIO17   | UART1 RX của ESP32 |
| RX    | GPIO18   | UART1 TX của ESP32 |
| WAKEUP| GPIO16   | Optional, pull-up |

### MFRC522 RFID (SPI)
| MFRC522 | ESP32-S3 | Ghi chú |
|---------|----------|---------|
| SDA/CS  | GPIO10   | Chip Select |
| SCK     | GPIO12   | SPI Clock |
| MOSI    | GPIO11   | |
| MISO    | GPIO13   | |
| GND     | GND      | |
| RST     | GPIO9    | |
| 3.3V    | 3.3V     | |

### Buzzer / LED Status
| Component | ESP32-S3 | Ghi chú |
|-----------|----------|---------|
| Buzzer+   | GPIO4    | PWM capable |
| LED Green | GPIO5    | Chấm công thành công |
| LED Red   | GPIO6    | Từ chối / lỗi |

### Display (I2C OLED 0.96")
| OLED | ESP32-S3 |
|------|----------|
| SDA  | GPIO8    |
| SCL  | GPIO7    |

Gợi ý: tạo file `include/pins.h` để define tất cả pin một chỗ.
