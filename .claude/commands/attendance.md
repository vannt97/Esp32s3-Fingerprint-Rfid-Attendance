Xem và phân tích dữ liệu chấm công từ project.

Thực hiện theo thứ tự:

1. **Tìm nguồn dữ liệu**: Kiểm tra xem project lưu log ở đâu:
   - File CSV/JSON trong SPIFFS: tìm trong code `SPIFFS.open(` hoặc `LittleFS.open(`
   - SD card: tìm `SD.open(`
   - Gửi qua Serial: tìm format log trong code
   - Server/API: tìm `http.POST(` hoặc `WiFiClient`

2. **Nếu có file log local**: Đọc và hiển thị dưới dạng bảng:
   ```
   STT | Nhân viên    | ID  | Phương thức | Thời gian           | Trạng thái
   ----|--------------|-----|-------------|---------------------|------------
   1   | Nguyễn Văn A | FP3 | Vân tay     | 2024-01-15 08:02:31 | Vào
   2   | Trần Thị B   | RC7 | Thẻ RFID    | 2024-01-15 08:15:44 | Vào
   ```

3. **Thống kê nhanh**:
   - Tổng lượt chấm công hôm nay
   - Ai đi muộn (sau 8:30)
   - Ai chưa chấm ra (nếu có log ra)

4. **Nếu chưa có dữ liệu**: Hỏi user muốn:
   a) Xem hướng dẫn format log chuẩn để implement
   b) Test với dữ liệu mẫu

Gợi ý format log CSV chuẩn: `timestamp,employee_id,method,card_uid_or_fp_id,direction`
