Compile firmware mà không upload — kiểm tra lỗi nhanh.

Thực hiện:
1. Chạy `pio run` trong thư mục project.
2. Nếu compile thành công: hiển thị RAM/Flash usage (lấy từ output pio).
3. Nếu có lỗi: liệt kê rõ từng lỗi theo format `file.cpp:line — mô tả lỗi`, nhóm theo file.
4. Nếu có warning quan trọng (unused variable, implicit cast, stack overflow risk): highlight ra.

Gợi ý fix cụ thể nếu lỗi là do thư viện thiếu hoặc sai version trong platformio.ini.
