# SEO đăng video Demo UI — Máy Chấm Công ESP32-S3 (Vân tay AS608 + RFID PN532)

> Áp dụng cho video dựng từ `docs/video-script-demo.md`.
> ⚠️ Giữ đúng định vị: đây là **demo giao diện / mô phỏng luồng thao tác đang phát triển**, KHÔNG phải sản phẩm chấm công đã hoàn thiện. Mọi tiêu đề/caption bên dưới đều tránh khẳng định "đã chấm công được thật" — dùng các từ như "demo", "thử nghiệm", "đang làm", "mô phỏng UI" để không bị coi là câu view sai sự thật (ảnh hưởng uy tín + dễ bị report/dislike khi người xem rành kỹ thuật soi ra).

---

## 1. Từ khoá gốc (dùng xuyên suốt title/description/hashtag)

**Từ khoá chính:**
- máy chấm công ESP32
- máy chấm công vân tay tự làm
- ESP32-S3 DIY
- cảm biến vân tay AS608
- module RFID PN532
- máy chấm công vân tay + thẻ từ

**Từ khoá phụ / đuôi dài (long-tail):**
- tự làm máy chấm công bằng ESP32-S3
- demo giao diện máy chấm công
- lập trình ESP32-S3 PlatformIO
- máy chấm công DIY vân tay RFID
- dự án IoT sinh viên ESP32
- màn hình OLED SSD1306 ESP32

**Đối tượng mục tiêu:** người học/làm điện tử-nhúng, sinh viên CNTT/Điện tử, cộng đồng Arduino/ESP32/PlatformIO Việt Nam, dân văn phòng tò mò về máy chấm công tự chế.

---

## 2. YouTube Shorts

**Tiêu đề (chọn 1, ưu tiên #1):**
1. Tự làm máy chấm công bằng ESP32-S3 | Demo giao diện #Shorts
2. Demo UI máy chấm công vân tay + RFID tự làm trên ESP32-S3
3. Làm máy chấm công mini với ESP32-S3 — Tập 1: Demo giao diện

**Mô tả (description):**
```
Demo giao diện (UI) máy chấm công tự làm trên ESP32-S3, dùng cảm biến vân tay AS608 + module RFID PN532 + màn OLED SSD1306.

⚠️ Đây là bản demo UI / mô phỏng luồng thao tác, cảm biến thật chưa được nối để nhận diện — video sau mình sẽ nối cảm biến thật để máy chấm công hoạt động thật sự.

Trong video:
- Luồng đăng ký vân tay (Enroll)
- Luồng đăng ký thẻ RFID
- Màn hình danh sách nhân viên

Theo dõi để xem phần nối cảm biến thật nhé!

#máychấmcông #ESP32S3 #DIY #maker #IoT #Arduino #PlatformIO #embedded #lậptrìnhnhúng #vântay #RFID
```

**Tags (phần Tags của YouTube Studio, không phải hashtag):**
```
esp32-s3, máy chấm công, may cham cong, esp32, arduino, diy electronics, iot việt nam, platformio, as608 fingerprint, pn532 rfid, oled ssd1306, lập trình nhúng, dự án sinh viên, maker việt nam, tự làm máy chấm công
```

**Hashtag hiển thị trên video (tối đa 3 cái đầu mô tả sẽ nổi bên tiêu đề):** `#Shorts #ESP32S3 #máychấmcông`

---

## 3. TikTok

**Caption (ngắn gọn, hook trước, hashtag sau):**
```
Demo giao diện máy chấm công mình tự làm bằng ESP32-S3 🔧 vân tay + thẻ RFID, chưa nối cảm biến thật nha, tập sau mình làm tiếp phần chấm công thật 👀

#esp32 #diy #maker #iot #machamcong #arduino #congngheviet #lậptrình #sinhviênkỹthuật #reviewcode
```

**Gợi ý text overlay mở đầu (2-3 giây đầu để giữ chân người xem):**
- "Máy chấm công mình đang tự làm 🛠️"
- "ESP32-S3 + vân tay + RFID — demo UI"

**Lưu ý thuật toán TikTok:**
- 3 giây đầu phải có chữ + hình rõ ràng (không chỉ voice-over) vì nhiều người xem tắt tiếng.
- Ưu tiên 5-7 hashtag, trộn hashtag rộng (`#diy #iot #arduino`) với hashtag ngách (`#esp32s3 #machamcong`) để vừa vào top ngách vừa có cơ hội lên xu hướng rộng.
- Đăng giờ vàng cộng đồng maker/dev VN: khoảng 20h-22h hoặc 12h-13h trưa các ngày trong tuần.

---

## 4. Facebook (trang cá nhân / fanpage)

**Nội dung post (dài hơn, kể chuyện, khuyến khích tương tác/comment):**
```
Mình đang tự làm một máy chấm công mini bằng ESP32-S3, dùng cảm biến vân tay AS608 và module RFID PN532, màn hình OLED để hiển thị giao diện.

Video này là bản demo giao diện + luồng thao tác mình đã code xong (Enroll vân tay, Enroll thẻ RFID, xem danh sách nhân viên). Cảm biến thật chưa nối vào để nhận diện — đây mới là bước hoàn thiện UI/UX trước khi bắt tay nối phần cứng thật.

Mọi người xem giao diện thế này ổn chưa, có góp ý gì cho mình luồng thao tác không? 👇

#ESP32S3 #máychấmcông #DIY #maker #IoT #PlatformIO
```

**Facebook Groups (nhóm maker/lập trình nhúng/Arduino/ESP32 Việt Nam):**
- Đăng bản rút gọn, đúng tinh thần chia sẻ kỹ thuật (không giống bài quảng cáo):
```
Chào mọi người, mình đang làm máy chấm công mini trên ESP32-S3 (AS608 vân tay + PN532 RFID + OLED SSD1306, code bằng PlatformIO).

Video demo phần giao diện + luồng thao tác (Enroll vân tay/thẻ, xem danh sách nhân viên) — cảm biến thật chưa nối, mới là bước làm UI trước. Mình muốn xin ý kiến mọi người về luồng thao tác, có gì cần chỉnh trước khi đi vào phần nối cảm biến thật không ạ? Cảm ơn mọi người!
```
- **Lưu ý khi đăng nhóm:**
  - Đọc rule từng nhóm trước (nhiều nhóm cấm/giới hạn video ngắn dạng "khoe", ưu tiên bài có nội dung kỹ thuật/học hỏi).
  - Không đăng y hệt một nội dung ở nhiều nhóm cùng lúc trong thời gian ngắn — dễ bị đánh dấu spam. Giãn cách vài giờ và chỉnh lại câu mở đầu cho mỗi nhóm.
  - Chủ động trả lời comment kỹ thuật (loại cảm biến, giá linh kiện, code ở đâu) — nhóm kỹ thuật tương tác tốt nhờ phần bình luận hơn là caption.
  - Gợi ý loại nhóm để đăng: nhóm về ESP32/Arduino, nhóm PlatformIO/lập trình nhúng, nhóm sinh viên Điện tử-Viễn thông/CNTT, nhóm Maker/DIY Việt Nam — tự tìm và xin duyệt tham gia, mình không có sẵn link nhóm cụ thể nên bạn chọn nhóm phù hợp đang tham gia nhé.

---

## 5. Việc chung cần làm trước khi đăng

- [ ] Thumbnail/cover cho YouTube Shorts: chụp màn hình OLED lúc hiện `Success!` hoặc màn `Enroll`, chữ to dễ đọc trên di động.
- [ ] Đặt tên file video khi export có chứa từ khoá, ví dụ: `demo-ui-may-cham-cong-esp32s3-vantay-rfid.mp4` (tên file cũng được các nền tảng đọc để gợi ý SEO).
- [ ] Pinned comment (YouTube/TikTok) nhắc lại đây là bản demo UI, mời người xem theo dõi phần nối cảm biến thật.
- [ ] Đồng bộ caption 3 nền tảng nhưng đừng copy y hệt — chỉnh độ dài/hashtag phù hợp từng nơi (đã soạn riêng ở trên).
