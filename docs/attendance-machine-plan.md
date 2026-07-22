# Máy chấm công: check-in trên thiết bị + server Django quản lý

Theo dõi tiến độ — tick `[x]` khi xong từng bước. Nội dung chi tiết đầy đủ
nằm ở plan gốc: `/Users/vannt/.claude/plans/sorted-cuddling-koala.md`.

## Context

Đã có luồng **enroll** (đăng ký vân tay/thẻ, lưu mapping `employee_id ↔
template_id/uid` vào NVS qua `EnrollmentStore`). Lần này làm 2 phần độc
lập, **chưa nối mạng với nhau**:

- **A. Check-in trên thiết bị** — `HomeScreen` tự động nhận diện vân
  tay/thẻ đã enroll, ghi log vào LittleFS ngay trên ESP32.
- **B. Server Django quản lý** — model + API tối thiểu (danh sách nhân
  viên, nhận log chấm công) + Django admin. Đặt tại `/server`. Test độc
  lập qua curl/admin, **chưa** nối firmware gọi API này (việc đó là task
  riêng sau: `ApiService` dùng `HTTPClient`, auth, hàng đợi offline).

Đã chốt: check-in tự động ở HomeScreen (không cần bấm nút), log lưu
LittleFS, Django tại `/server` dùng SQLite, phạm vi API tối thiểu + admin
có sẵn (không làm dashboard riêng), chưa nối firmware↔Django lần này.

---

## Phần A — Check-in trên thiết bị (ESP32)

- [ ] **A1.** `TimeManager::getEpoch()` — trả epoch Unix, `0` nếu chưa
      sync NTP (`src/attendance/services/TimeManager.h/.cpp`)
- [ ] **A2.** `FingerprintService::verifyStep()` — chế độ nhận diện
      (khác enroll): `getImage()` → `image2Tz()` → `fingerSearch()`, trả
      `FingerVerifyResult{NO_FINGER, MATCHED, NOT_FOUND, ERROR}`
      (`src/attendance/services/FingerprintService.h/.cpp`)
- [ ] **A3.** `EnrollmentStore` — thêm key ngược `"t<templateId>"` →
      `employeeId` khi `saveFingerMapping()`, thêm
      `findEmployeeByTemplateId()` (`src/attendance/services/EnrollmentStore.h/.cpp`)
- [ ] **A4.** Service mới `AttendanceLog` (LittleFS, `/attendance.log`
      CSV append) — `begin()`, `append(employeeId, method, epoch)`,
      `dumpToSerial()` (`src/attendance/services/AttendanceLog.h/.cpp`, mới)
- [ ] **A5.** Wire `AttendanceLog` vào `ScreenManager`/`main.cpp`, thêm
      `pinMode(LED_GREEN/LED_RED, OUTPUT)`, bật
      `board_build.filesystem = littlefs` cho `env:attendance` trong
      `platformio.ini`
- [ ] **A6.** `HomeScreen` — state `CLOCK`/`RESULT`, poll
      `verifyStep()`/`pollCard()` mỗi tick lúc `CLOCK` (throttle poll thẻ
      150ms), banner tên+kết quả 2.5s lúc `RESULT` (debounce tự nhiên vì
      không poll khi đang hiện banner), ghi `AttendanceLog` khi nhận diện
      thành công, bật LED xanh/đỏ tương ứng
      (`src/attendance/screens/HomeScreen.h/.cpp`)
- [ ] **Build & test phần A** — `pio run -e attendance`, flash, test theo
      mục Kiểm thử trong plan gốc (đặt tay/thẻ đã enroll → banner đúng
      tên; tay/thẻ lạ → "Not registered"; reset board → log còn nguyên)

## Phần B — Server Django quản lý chấm công

- [ ] **B1.** Khởi tạo Django project `/server` (`django-admin
      startproject config .` + `startapp attendance`, venv riêng,
      `requirements.txt`, `.gitignore`)
- [ ] **B2.** Models: `Employee` (name, is_active, created_at), `Device`
      (user 1-1, name, last_seen_at), `AttendanceRecord` (employee FK,
      device FK, method F/C, event_time, client_record_id nullable +
      unique_together với device, received_at)
- [ ] **B3.** API (Django REST Framework + token auth):
      `GET /api/employees/`, `POST /api/attendance/` (idempotent theo
      `client_record_id` — trùng thì trả bản ghi cũ thay vì lỗi)
- [ ] **B4.** Đăng ký Django admin cho cả 3 model (list_display,
      list_filter, search_fields)
- [ ] **B5.** Migrate, tạo superuser, tạo `User`+`Device` mẫu, lấy token
      bằng `drf_create_token`, test bằng curl theo mục Kiểm thử trong plan
      gốc (GET employees, POST attendance, POST trùng client_record_id
      không tạo bản ghi mới)

## Giới hạn đã biết (chưa xử lý lần này)

- WiFi/NTP chưa sync lúc chấm công → epoch ghi 0.
- Tra thẻ (RFID) trên thiết bị bằng quét tuyến tính 5 nhân viên mock —
  cần đổi khi có danh sách thật từ `/api/employees/`.
- `/attendance.log` trên thiết bị chỉ tăng dần, chưa dọn/đồng bộ.
- Chưa nối firmware gọi API Django (task riêng sau, cần `ApiService` +
  HTTPS + hàng đợi offline).
- Chưa deploy production cho Django (chạy `runserver` cho dev/test).
