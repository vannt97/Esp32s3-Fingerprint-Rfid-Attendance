Build và upload firmware lên ESP32-S3, sau đó mở serial monitor.

Thực hiện tuần tự:
1. Chạy `pio run -t upload` trong thư mục project. Nếu lỗi, phân tích lỗi compile/link và báo cáo rõ file:line.
2. Nếu upload thành công, chạy `pio device monitor --baud 115200` và hiển thị output 10 giây đầu.
3. Tóm tắt: upload thành công hay thất bại, và bất kỳ warning đáng chú ý nào.

Nếu không tìm thấy board (upload error), gợi ý kiểm tra: cáp USB, driver CH340/CP2102, port trong platformio.ini.
