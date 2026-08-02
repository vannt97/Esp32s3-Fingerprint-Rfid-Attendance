#!/bin/sh
set -e

python manage.py migrate --noinput

# Tự tạo admin đầu tiên nếu có khai báo biến môi trường (xem .env.example).
# --noinput đọc DJANGO_SUPERUSER_USERNAME/EMAIL/PASSWORD, Django hỗ trợ sẵn.
# Bỏ qua lỗi nếu user đã tồn tại (idempotent qua các lần restart container).
if [ -n "$DJANGO_SUPERUSER_USERNAME" ] && [ -n "$DJANGO_SUPERUSER_PASSWORD" ]; then
    python manage.py createsuperuser --noinput 2>/dev/null || true
fi

exec python manage.py runserver 0.0.0.0:8000
