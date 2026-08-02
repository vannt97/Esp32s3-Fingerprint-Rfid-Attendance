Copy file mp3 của bạn vào thư mục này với tên chính xác:

    xacthuc.mp3

Sau đó upload filesystem lên ESP32-S3 (chỉ cần làm lại khi đổi file audio,
không cần build lại firmware):

    pio run -e test_audio -t uploadfs

Rồi build + flash + mở serial monitor như bình thường:

    pio run -e test_audio -t upload -t monitor
