Mở serial monitor để debug ESP32-S3.

Thực hiện:
1. Chạy `pio device monitor --baud 115200 --filter colorize` (thêm timestamp nếu có filter đó).
2. Nếu không kết nối được: kiểm tra port bằng `pio device list` và gợi ý port đúng.
3. Hướng dẫn user: nhấn Ctrl+C để thoát monitor.

Lưu ý cho user: nếu muốn filter log theo loại (FINGERPRINT / RFID / ATTENDANCE), dùng `pio device monitor | grep -E "FP:|RFID:|ATT:"`.
