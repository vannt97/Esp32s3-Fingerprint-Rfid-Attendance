# Kịch bản Video Short — Demo UI / Mô phỏng luồng hoạt động Máy Chấm Công ESP32-S3 (Vân tay + RFID)

> Thời lượng đề xuất: 45–60 giây
> Định dạng: dọc 9:16 (quay màn hình OLED cận cảnh + tay bấm nút)
> ⚠️ Định vị nội dung: đây là **demo giao diện / mô phỏng luồng thao tác**, KHÔNG phải video khoe máy chấm công đã hoàn thiện. Cảm biến AS608 và đầu đọc RFID hiện chưa được nối để nhận diện thật — mỗi bước "quét" trong Enroll đang được giả lập bằng cách bấm nút SELECT. Toàn bộ voice-over/text overlay bên dưới phải giữ đúng tinh thần "đây là bản demo UI/luồng đang phát triển", tránh mọi câu khẳng định máy đã chấm công được thật.
> Lưu ý: toàn bộ text hiển thị dưới đây lấy đúng từ code hiện tại (EnrollScreen, FingerScanScreen, CardScanScreen, UsersScreen) — quay đúng như vậy sẽ khớp 100% với thiết bị thật.

---

## Chuẩn bị trước khi quay
- Board ESP32-S3 + màn OLED SSD1306 + cảm biến AS608 + đầu đọc RFID đã lắp sẵn, đứng yên trên bàn/giá đỡ.
- Ánh sáng đủ để cận cảnh màn OLED không bị lóa (tắt đèn flash, dùng đèn mềm chéo góc).
- Gắn kèm 1 điện thoại quay macro màn hình + 1 góc quay tay bấm nút (có thể quay 2 lần, ghép sau).
- Nhạc nền: nhịp nhanh, không lời, kiểu "tech/gadget demo".

---

## SCENE 1 — Mở đầu / Hook (0:00–0:04)
**Hình ảnh:** Cận cảnh toàn bộ thiết bị, màn hình đang ở màn `Employee`.
**Text overlay:** "Demo UI máy chấm công ESP32-S3 mình đang làm 👀"
**Voice-over (tuỳ chọn):** "Đây là bản demo giao diện, mô phỏng luồng thao tác cho máy chấm công vân tay và thẻ RFID mình đang phát triển."

---

## SCENE 2 — Vào menu Enroll (0:04–0:08)
**Thao tác:** Từ màn `Employee`, bấm SELECT vào mục đầu tiên → vào màn `Enroll`.
**Màn hình hiển thị:**
```
Enroll
------------------
[Fingerprint]
[Card]
                Exit
```
**Text overlay:** "Bước 1: Chọn phương thức đăng ký"

---

## SCENE 3 — Đăng ký vân tay (0:08–0:24)
**Thao tác:** Chọn `Fingerprint` → SELECT → vào màn chọn ngón tay → vào `FingerScanScreen`.

**Chuỗi màn hình (bấm SELECT sau mỗi bước để giả lập cảm biến quét xong):**

| Bước | Nội dung hiển thị |
|---|---|
| 1 | `Place finger` / `(1/3)` |
| 2 | `Remove finger...` |
| 3 | `Place finger again` / `(2/3)` |
| 4 | `Remove finger...` |
| 5 | `Place finger again` / `(3/3)` |
| 6 | `Saving...` |
| 7 | `Success!` ✅ |

**Text overlay theo nhịp:** "Luồng đăng ký vân tay (mô phỏng) — đặt tay 3 lần theo thiết kế"
**Voice-over:** "Đây là luồng đăng ký vân tay mình thiết kế: đặt ngón tay ba lần liên tiếp, mỗi bước hiện đang được giả lập bằng nút SELECT vì cảm biến AS608 chưa nối để đọc thật."
**Ghi chú quay:** Zoom cận ngón tay đặt lên cảm biến AS608 đúng lúc màn hình đổi từ "Remove finger..." sang "Place finger again" để tạo cảm giác đồng bộ hình ảnh — nhưng lưu ý voice-over/overlay phải nói rõ đây là bấm nút SELECT mô phỏng, không phải cảm biến thật đang đọc.

**Kết thúc scene:** Màn hình hiện `Success!` → tự động quay lại màn chọn ngón tay.

---

## SCENE 4 — Đăng ký thẻ RFID (0:24–0:38)
**Thao tác:** Từ `Enroll`, chọn `Card` → SELECT → vào `CardScanScreen`.

**Chuỗi màn hình (bấm SELECT sau mỗi bước):**

| Bước | Nội dung hiển thị |
|---|---|
| 1 | `Card Number` / `Place card near` / `reader` |
| 2 | `Reading...` |
| 3 | `Card detected:` / `AB:12:CD:34` *(UID ngẫu nhiên)* |
| 4 | `Saving...` |
| 5 | `Success!` ✅ *(hoặc `Error: Card already used` — có xác suất 50%, nên quay lại nếu ra lỗi)* |

**Text overlay:** "Luồng đăng ký thẻ RFID (mô phỏng)"
**Voice-over:** "Tương tự, đây là luồng đăng ký thẻ mình thiết kế trên giao diện — đầu đọc RFID chưa nối thật nên UID và kết quả hiện đang random để demo hai nhánh Success/Error."
**Ghi chú quay:** Đặt thẻ RFID lên đầu đọc đúng lúc bấm SELECT ở bước "Place card near reader" để hình ảnh khớp nhịp — nhưng vẫn cần nói rõ đây là mô phỏng, không phải đầu đọc thật đang nhận UID. Nếu lần đầu bị `Error: Card already used`, cứ quay lại thao tác — chỉ cần giữ lại đoạn ra `Success!` khi dựng.

---

## SCENE 5 — Danh sách người dùng (0:38–0:50)
**Thao tác:** Từ `Employee`, chọn mục thứ hai → SELECT → vào màn `Users`.
**Thao tác trên màn:** Bấm UP/DOWN để cuộn qua danh sách, cho thấy thanh scrollbar bên phải chạy theo.
**Màn hình hiển thị:**
```
Users
------------------
[Nguyen Van A]      <- item đang chọn (highlight nền trắng)
 Tran Thi B
 Le Van C
 ...
                Exit
```
**Text overlay:** "Màn hình danh sách nhân viên (dữ liệu mẫu)"
**Voice-over:** "Đây là màn hình danh sách nhân viên trong thiết kế UI, cuộn bằng 2 nút bấm — hiện đang hiển thị dữ liệu mẫu dựng sẵn, chưa phải danh sách đăng ký thật."
**Ghi chú quay:** Bấm DOWN liên tục 4–5 lần để thấy list cuộn mượt + scrollbar di chuyển, đây là điểm nhấn hình ảnh đẹp cho short.

---

## SCENE 6 — Kết (0:50–0:55)
**Hình ảnh:** Cận cảnh toàn bộ thiết bị, có thể xoay nhẹ để khoe cảm biến vân tay + đầu đọc thẻ + màn OLED cùng lúc.
**Text overlay:** "Mới xong phần UI — sắp tới nối cảm biến thật 🛠️"
**Voice-over:** "Đây mới là bản demo giao diện và luồng thao tác. Bước tiếp theo mình sẽ nối cảm biến vân tay và đầu đọc RFID thật để máy chấm công thật sự hoạt động."
**CTA:** "Follow để theo dõi mình nối cảm biến thật cho máy chấm công nhé!"

---

## Ghi chú kỹ thuật (để không quay sai luồng và không nói quá về sản phẩm)
- Nút điều khiển: `UP` / `DOWN` để di chuyển menu hoặc cuộn danh sách, `SELECT` để xác nhận/chuyển bước, `EXIT` để quay lại màn trước.
- **Định vị bắt buộc:** video này là demo UI/mô phỏng luồng thao tác, KHÔNG phải video khoe "máy chấm công đã hoàn thiện". Tránh mọi câu chữ khẳng định máy đã chấm công/nhận diện vân tay-thẻ thật.
- Toàn bộ luồng Enroll vân tay và Card hiện tại là **mô phỏng giao diện** (chưa nối cảm biến AS608/PN532 thật) — mỗi lần bấm SELECT là giả lập một bước cảm biến đã quét xong. Kết quả `Success!` / `Error` ra ngẫu nhiên 50/50, nên có thể cần bấm lại vài lần trong lúc quay để lấy được đúng kịch bản "thành công" mong muốn.
- Danh sách `Users` là dữ liệu mẫu cố định trong code (`USER_NAMES`), không phải dữ liệu đăng ký thật từ Scene 3–4.
- Chưa có luồng "chấm công" thật (check-in bằng vân tay/thẻ để ghi nhận giờ vào-ra) trong code hiện tại — chỉ mới có Employee menu → Enroll (đăng ký) và Users (xem danh sách). Nếu về sau làm luồng check-in thật thì mới nên gọi video là "demo chấm công".

---

## Danh sách shot cần quay (checklist dựng phim)
- [ ] Cận cảnh thiết bị tổng thể (mở đầu + kết thúc)
- [ ] Màn `Enroll` với 2 mục Fingerprint / Card
- [ ] Toàn bộ 7 bước màn hình `FingerScanScreen` (ưu tiên lấy bản ra `Success!`)
- [ ] Toàn bộ 5 bước màn hình `CardScanScreen` (ưu tiên lấy bản ra `Success!`)
- [ ] Đặt tay/thẻ khớp nhịp với đổi màn hình (quay riêng, ghép sau nếu cần)
- [ ] Màn `Users` đang cuộn qua danh sách
- [ ] B-roll: tay bấm nút UP/DOWN/SELECT/EXIT cận cảnh
