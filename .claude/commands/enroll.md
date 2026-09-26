Hướng dẫn quy trình enroll vân tay mới trên cảm biến AS608.

Hiển thị checklist và giải thích quy trình enroll AS608:

## Chuẩn bị
- Board đã được flash firmware với chức năng enroll (kiểm tra main.cpp có hàm enrollFingerprint() không)
- Cảm biến AS608 kết nối đúng: TX→RX, RX→TX, VCC 3.3V, GND

## Quy trình AS608 (2 lần quét)
1. **Lần 1**: Đặt ngón tay → LED xanh nhấp nháy → lấy ảnh → tạo CharBuffer1
2. **Nhấc tay** → chờ LED tắt
3. **Lần 2**: Đặt lại ngón tay → lấy ảnh → tạo CharBuffer2  
4. **So khớp** CharBuffer1 + CharBuffer2 → tạo template → lưu vào ID slot

## Các lỗi thường gặp và cách fix
| Lỗi | Nguyên nhân | Fix |
|-----|-------------|-----|
| PACKETRECIEVEERR | Baud rate sai | Kiểm tra baud AS608 (mặc định 57600) |
| IMAGEMESS | Ngón tay bẩn/ướt | Lau khô ngón tay |
| FEATUREFAIL | Ảnh quá mờ | Ấn ngón tay chắc hơn vào cảm biến |
| INVALIDIMAGE | Không có ngón tay | Đặt ngón tay đúng vị trí |
| FINGERPRINTPACKETRECIEVEERR | Wiring lỗi | Kiểm tra lại TX/RX đảo chưa |

## Sau khi enroll
- Mỗi ngón tay có thể lưu ở nhiều ID slot (dự phòng)
- AS608 lưu được tối đa 127 template
- Ghi chú ID → tên nhân viên vào database (SPIFFS/SD/server)

Hỏi user: enroll cho nhân viên nào, ID slot nào sẽ dùng? Sau đó kiểm tra code có hàm enroll chưa.
