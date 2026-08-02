# Attendance server

Django + Django REST Framework, SQLite. Xem chi tiết thiết kế ở
`../docs/attendance-machine-plan.md` (Phần B).

## Chạy bằng Docker (khuyến nghị — chỉ cần cài Docker)

```bash
cd server
docker compose up
```

Lần đầu chạy sẽ tự động: build image, `migrate`, tạo tài khoản admin từ
`.env` (mặc định `admin` / `admin123456` — **đổi trước khi dùng thật**),
rồi chạy server tại `http://localhost:8000/`. Dữ liệu (`db.sqlite3`) nằm
ngay trong thư mục `server/` (mount volume), không mất khi tắt/bật lại
container.

- Admin: `http://localhost:8000/admin/`
- API: `http://localhost:8000/api/employees/`, `http://localhost:8000/api/attendance/`

Sửa code Python thì chỉ cần lưu file — server tự reload (đang chạy
`runserver`, không cần build lại image, vì thư mục được mount trực tiếp).

Dừng: `docker compose down` (không xoá `db.sqlite3`, chỉ tắt container).

## Chạy không dùng Docker (venv)

```bash
cd server
python3 -m venv venv
./venv/bin/pip install -r requirements.txt
./venv/bin/python manage.py migrate
./venv/bin/python manage.py createsuperuser
./venv/bin/python manage.py runserver
```

## Lấy token cho thiết bị

```bash
./venv/bin/python manage.py drf_create_token <device_username>
# hoặc qua Docker:
docker compose exec web python manage.py drf_create_token <device_username>
```
