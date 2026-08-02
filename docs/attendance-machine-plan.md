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

- [x] **A1.** `TimeManager::getEpoch()` — trả epoch Unix, `0` nếu chưa
      sync NTP (`src/attendance/services/TimeManager.h/.cpp`)
- [x] **A2.** `FingerprintService::verifyStep()` — chế độ nhận diện
      (khác enroll): `getImage()` → `image2Tz()` → `fingerSearch()`, trả
      `FingerVerifyResult{NO_FINGER, MATCHED, NOT_FOUND, ERROR}`
      (`src/attendance/services/FingerprintService.h/.cpp`)
- [x] **A3.** `EnrollmentStore` — thêm key ngược `"t<templateId>"` →
      `employeeId` khi `saveFingerMapping()`, thêm
      `findEmployeeByTemplateId()` (`src/attendance/services/EnrollmentStore.h/.cpp`)
- [x] **A4.** Service mới `AttendanceLog` (LittleFS, `/attendance.log`
      CSV append) — `begin()`, `append(employeeId, method, epoch)`,
      `dumpToSerial()` (`src/attendance/services/AttendanceLog.h/.cpp`, mới)
- [x] **A5.** Wire `AttendanceLog` vào `ScreenManager`/`main.cpp`, thêm
      `pinMode(LED_GREEN/LED_RED, OUTPUT)`, bật
      `board_build.filesystem = littlefs` cho `env:attendance` trong
      `platformio.ini`
- [x] **A6.** `HomeScreen` — state `CLOCK`/`RESULT`, poll
      `verifyStep()`/`pollCard()` mỗi tick lúc `CLOCK` (throttle poll thẻ
      150ms), banner tên+kết quả 2.5s lúc `RESULT` (debounce tự nhiên vì
      không poll khi đang hiện banner), ghi `AttendanceLog` khi nhận diện
      thành công, bật LED xanh/đỏ tương ứng
      (`src/attendance/screens/HomeScreen.h/.cpp`)
- [x] **Build & test phần A** — `pio run -e attendance` build pass
      (RAM 14.3%, Flash 29.6%). **Chưa test trên phần cứng thật** — cần tự
      flash và đi qua mục Kiểm thử trong plan gốc (đặt tay/thẻ đã enroll →
      banner đúng tên; tay/thẻ lạ → "Not registered"; reset board → log
      còn nguyên).

## Phần B — Server Django quản lý chấm công

- [x] **B1.** Khởi tạo Django project `/server` (`django-admin
      startproject config .` + `startapp attendance`, venv riêng,
      `requirements.txt`, `.gitignore`)
- [x] **B2.** Models: `Employee` (name, is_active, created_at), `Device`
      (user 1-1, name, last_seen_at), `AttendanceRecord` (employee FK,
      device FK, method F/C, event_time, client_record_id nullable +
      unique_together với device, received_at)
- [x] **B3.** API (Django REST Framework + token auth):
      `GET /api/employees/`, `POST /api/attendance/` (idempotent theo
      `client_record_id` — trùng thì trả bản ghi cũ thay vì lỗi)
- [x] **B4.** Đăng ký Django admin cho cả 3 model (list_display,
      list_filter, search_fields)
- [x] **B5.** Migrate, tạo superuser, tạo `User`+`Device` mẫu, lấy token
      bằng `drf_create_token`, test bằng curl — đã xác nhận: GET employees
      trả đúng JSON, POST attendance trả 201 lần đầu / 200 lần lặp lại
      cùng `client_record_id` (không tạo bản ghi mới), request thiếu token
      trả 401. DB cuối cùng có đúng 2 bản ghi test.

## Phần C — Sửa nền tảng (WiFi reconnect loop, time sync retry)

- [x] **C1.** `main.cpp`: thêm `wifiManager.loop()` vào `loop()` (trước đây
      chưa từng được gọi — state machine reconnect bị đứng sau boot).
- [x] **C2.** `TimeManager::begin()`: đổi sang `configTzTime("<+07>-7", ...)`
      tường minh thay vì dựa hành vi ngầm của `configTime()`.
- [x] **C3.** `main.cpp`: cờ `timeSyncStarted`, thử `timeManager.begin()`
      ngay lần đầu WiFi kết nối được sau boot (không chỉ đúng lúc `setup()`).

## Phần D — `AttendanceLog`: đọc để phục vụ đồng bộ

- [x] **D1.** Thêm `size()`/`readRange(fromOffset, maxBytes)`.
- [x] **D2.** `append()` ghi 1 lần bằng buffer cục bộ (thay vì nhiều
      `f.printf()`); `begin()` cắt bỏ dòng cuối dở dang nếu có (mất điện
      giữa lúc ghi).

## Phần E — `EmployeeStore`: đồng bộ danh sách nhân viên (thay mock)

- [x] **E1.** Service mới `EmployeeStore` (`src/attendance/services/`) —
      cache LittleFS `/employees.json`, seed từ `EMPLOYEE_SEED[]`
      (`EmployeeData.h`, đổi tên từ `EMPLOYEES[]`) nếu chưa từng sync.
- [x] **E2.** Cập nhật `UsersScreen`, `EmployeeSelectScreen`, `HomeScreen`,
      `main.cpp` (debug dump) sang gọi `ScreenManager::getEmployeeStore()`
      thay vì mảng mock trực tiếp.
- [x] **E3.** Wire vào `ScreenManager` (tham số constructor + getter).

## Phần F — `ApiService`: đẩy log chấm công + hàng đợi offline

- [x] **F1.** `include/config.h` thêm `API_BASE_URL`, `API_DEVICE_TOKEN`,
      `API_ATTENDANCE_SYNC_INTERVAL_MS` (~60s), `API_EMPLOYEE_REFETCH_INTERVAL_MS` (~30 phút).
- [x] **F2.** Service mới `ApiService` — `fetchEmployees()` (GET, stream
      thẳng vào `EmployeeStore`), `pushAttendanceRecord()` (POST, ISO8601
      UTC + `client_record_id`), `syncPendingAttendance()` (đọc tối đa ~10
      bản ghi/lần từ `AttendanceLog`, dừng ngay khi gửi lỗi, chỉ lưu tiến
      độ offset sau mỗi dòng gửi thành công). Mọi hàm gate theo
      `WiFi.isConnected()` + timeout ngắn (2-3s) để không đứng UI khi mất
      mạng/server sập.
- [x] **F3.** Wire vào `main.cpp` (fetch lúc boot + 2 `Timeout` định kỳ)
      và `HomeScreen::_showResult()` (đẩy ngay sau mỗi lần chấm công,
      dùng chung hàm với vòng thử lại định kỳ).

## Phần G — Xóa enrollment (vân tay/thẻ)

- [x] **G1.** `FingerprintService::deleteTemplate()` (bọc AS608
      `deleteModel()`), `EnrollmentStore::removeFingerMapping()` /
      `removeCardMapping()` (xóa cả key thuận lẫn ngược).
- [x] **G2.** `EnrollTarget::DELETE`, `ScreenId::DELETE_ENROLLMENT`,
      `EmployeeScreen` thêm mục "Delete" (`ITEM_COUNT` 2→3).
- [x] **G3.** `EmployeeSelectScreen` — sửa từ rẽ nhị phân sang switch 3
      nhánh tường minh (SELECT), EXIT có điều kiện theo target (DELETE →
      `EMPLOYEE`, còn lại → `ENROLL`) — bắt buộc sửa, không thì DELETE rơi
      nhầm vào luồng CARD_SCAN.
- [x] **G4.** Màn hình mới `DeleteEnrollmentScreen` — danh sách ĐỘNG (chỉ
      liệt kê đúng finger/card nhân viên đã chọn thực sự có), chọn 1 mục
      → xác nhận (SELECT=Yes/EXIT=No) → xóa cả AS608 lẫn NVS → "Deleted!"
      → dựng lại danh sách. Đăng ký vào `ScreenManager`.

## Phần H — Phản hồi âm thanh khi chấm công

- [x] **H1.** Dọn `data/` — chuyển ảnh/README sang `docs/screenshots/`
      (chỉ còn 2 file mp3 để `uploadfs` không đẩy nhầm ~6MB ảnh lên LittleFS).
- [x] **H2.** `include/pins.h` — đổi `LED_GREEN`/`LED_RED` sang GPIO 15/16
      (hết đụng chân I2S của loa MAX98357A, vốn dùng GPIO 5/6).
- [x] **H3.** `platformio.ini` — thêm `esphome/ESP32-audioI2S` vào
      `env:attendance`.
- [x] **H4.** Service mới `AudioFeedback` (bọc thư viện `Audio`) —
      `playSuccess()`/`playFailure()`, `loop()` gọi mỗi tick không gate
      theo màn hình. Gọi từ `HomeScreen::_showResult()`.

## Giới hạn đã biết (chưa xử lý)

- `API_BASE_URL` phải sửa tay đúng IP LAN thật của máy chạy Django —
  không có cơ chế tự dò (mDNS/service discovery).
- HTTP thường (không TLS) giữa ESP32↔Django — chỉ an toàn trong LAN riêng
  tin cậy.
- `allocateNextTemplateId()` không tái sử dụng ID sau khi xóa (bộ đếm chỉ
  tăng) — enroll/xóa lặp lại nhiều lần có thể báo "Storage full" dù còn
  ít vân tay thực tế. Cần API cấp thấp hơn (`nvs_entry_find`) mới tái chế
  được ID — ngoài phạm vi hiện tại.
- Chỉ tính cho 1 thiết bị — template ID trên AS608 đánh số cục bộ riêng
  từng máy.
- Mapping vân tay/thẻ ↔ nhân viên vẫn chỉ nằm trên thiết bị, KHÔNG đẩy
  lên Django — mất thiết bị/reset NVS thì phải enroll lại toàn bộ.
- Chưa deploy production cho Django (chạy `runserver`/`docker compose up`
  cho dev/nội bộ).
- **Tất cả Phần C–H mới chỉ build pass, CHƯA test trên phần cứng thật** —
  xem mục Kiểm thử trong plan gốc
  (`/Users/vannt/.claude/plans/sorted-cuddling-koala.md`) để tự flash và
  đi qua từng bước.
