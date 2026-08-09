# Kịch bản Video Short — ESP32-S3 Chấm Công Tự Động + Đồng Bộ Server Django

> Thời lượng đề xuất: 40–55 giây
> Định dạng: dọc 9:16 (quay màn hình OLED cận cảnh + tay đặt vân tay/thẻ + màn hình laptop Django admin)
> ✅ Định vị nội dung: tính năng **chấm công thật + đồng bộ server thật đã chạy ổn trên phần cứng** — khác với video demo UI trước đây (`docs/video-script-demo.md`), video này được phép khẳng định "đã chạy được", không cần né tránh.
> Toàn bộ text/trạng thái lấy đúng từ code hiện tại (`HomeScreen.cpp`, `ApiService`, Django admin) — quay đúng như vậy sẽ khớp 100% máy thật.

---

## Chuẩn bị trước khi quay

- Board ESP32-S3 + OLED + AS608 + đầu đọc RFID đã enroll sẵn ít nhất 1-2 nhân viên (vân tay + thẻ).
- Board đã kết nối WiFi, đã sync giờ (icon wifi trên màn Home hiện đủ sóng).
- Server Django đã chạy sẵn (`docker compose up` trong `server/`), có thể truy cập `http://<ip-lan>:8000/admin/` từ laptop dùng để quay.
- Laptop/màn hình mở sẵn tab Django admin → `Attendance records`, để trống hoặc filter theo hôm nay, sẵn sàng bấm refresh.
- Quay 2 góc: (1) cận màn OLED + tay đặt vân tay/thẻ, (2) màn hình laptop admin. Ghép lại lúc dựng.
- Nhạc nền: nhịp nhanh, không lời, kiểu "tech/gadget".

---

## SCENE 1 — Hook (0:00–0:04)

**Hình ảnh:** Cận cảnh thiết bị, màn hình đang ở Home (đồng hồ + icon wifi full).
**Text overlay:** "Máy chấm công ESP32-S3 của mình đã chạy thật rồi 🔥"
**Voice-over (tuỳ chọn):** "Đây là máy chấm công mình tự làm bằng ESP32-S3 — vân tay và thẻ RFID đều nhận diện được, và dữ liệu tự động đẩy lên server."

---

## SCENE 2 — Chấm công bằng vân tay (0:04–0:14)

**Thao tác:** Đặt ngón tay đã enroll lên cảm biến AS608.
**Màn hình đổi từ:**
```
   09:41
 07/08/2026
     Menu
```
→ sau ~1-2s hiện:
```
Nguyen Van A
Checked in!
```
**Hình ảnh đi kèm:** Đèn LED xanh sáng lên, loa phát âm thanh báo thành công.
**Text overlay:** "Chạm vân tay — nhận diện tức thì"
**Voice-over:** "Chỉ cần đặt tay lên cảm biến, máy nhận diện ngay và báo tên nhân viên."

---

## SCENE 3 — Chấm công bằng thẻ RFID (0:14–0:22)

**Thao tác:** Đưa thẻ RFID đã enroll lại gần đầu đọc.
**Màn hình hiện:**
```
Tran Thi B
Checked in!
```
**Hình ảnh đi kèm:** LED xanh, âm thanh thành công (giống Scene 2).
**Text overlay:** "Không có vân tay thì quẹt thẻ cũng được"
**Voice-over:** "Máy hỗ trợ song song 2 cách — vân tay hoặc thẻ, ai tiện cách nào dùng cách đó."

**Ghi chú quay (case lỗi, tuỳ chọn thêm nếu muốn cho thấy tính đầy đủ):** Đưa 1 thẻ/vân tay CHƯA enroll → màn hình hiện `Not registered`, LED đỏ, âm thanh lỗi. Có thể chèn nhanh 1-2s để chứng minh máy phân biệt được người lạ.

---

## SCENE 4 — Cắt cảnh sang server (0:22–0:34)

**Hình ảnh:** Chuyển góc quay sang màn hình laptop, tab Django admin đang mở sẵn `Attendance records`.
**Thao tác:** Bấm refresh (F5) ngay sau khi Scene 2/3 vừa "Checked in!" — bản ghi mới xuất hiện ở đầu danh sách với đúng tên nhân viên, phương thức (F/C), thời gian.
**Text overlay:** "Dữ liệu tự bay lên server — không cần thao tác gì thêm"
**Voice-over:** "Ngay khi chấm công xong, ESP32 tự động gửi dữ liệu qua WiFi lên server Django — mở admin lên là thấy bản ghi mới ngay lập tức."

**Ghi chú kỹ thuật để quay đúng nhịp:** `ApiService` đẩy log ngay trong `HomeScreen::_showResult()` sau mỗi lần chấm công thành công (không cần chờ chu kỳ đồng bộ định kỳ), nên refresh admin ngay sau khi banner "Checked in!" hiện là sẽ thấy bản ghi mới — canh quay 2 góc gần sát giờ nhau để dựng khớp.

---

## SCENE 5 — Kết / CTA (0:34–0:45)

**Hình ảnh:** Cận thiết bị + chèn nhanh màn admin đang hiện danh sách bản ghi.
**Text overlay:** "Tự làm máy chấm công từ A-Z: ESP32-S3 + vân tay + RFID + server riêng"
**Voice-over:** "Từ phần cứng đến server, mình tự code toàn bộ bằng ESP32-S3 và Django. Follow để xem mình cải tiến thêm những gì nhé!"
**CTA:** "Follow/theo dõi để xem thêm các bản cập nhật máy chấm công này 👇"

---

## Ghi chú kỹ thuật (để quay đúng luồng)

- Text hiển thị khi chấm công thành công: `"<Tên nhân viên>"` + `"Checked in!"` (đúng 2 dòng, dòng trên tên, dòng dưới trạng thái).
- Khi không nhận diện được: `"Not registered"` (không có dòng tên), LED đỏ, âm thanh lỗi.
- LED xanh (GPIO 15) / đỏ (GPIO 16) tự tắt sau 2.5s khi màn hình quay lại đồng hồ.
- Server: `docker compose up` trong `server/`, admin tại `/admin/`, model liên quan là `AttendanceRecord` (nhân viên, thiết bị, phương thức F/C, giờ chấm công, giờ server nhận).
- **Giới hạn cần biết khi trả lời comment kỹ thuật (tránh nói quá):** HTTP thường (chưa TLS), chỉ dùng trong mạng LAN riêng; địa chỉ server (`API_BASE_URL`) hiện phải cấu hình tay theo IP LAN, chưa tự dò; mapping vân tay/thẻ ↔ nhân viên vẫn lưu trên thiết bị (mất thiết bị phải enroll lại) — có thể trả lời nếu người xem hỏi sâu, không cần đưa vào script chính.

---

## Danh sách shot cần quay (checklist dựng phim)

- [ ] Cận cảnh thiết bị tổng thể lúc mở đầu (màn Home, icon wifi full)
- [ ] Đặt vân tay đã enroll → banner "Checked in!" + LED xanh + âm thanh
- [ ] Quẹt thẻ RFID đã enroll → banner "Checked in!" + LED xanh + âm thanh
- [ ] (Tuỳ chọn) 1 lượt vân tay/thẻ lạ → "Not registered" + LED đỏ + âm thanh lỗi
- [ ] Màn hình laptop Django admin, refresh thấy bản ghi mới xuất hiện đúng lúc
- [ ] B-roll: toàn cảnh setup (board + laptop + WiFi router) để nhấn mạnh "hệ thống end-to-end"
