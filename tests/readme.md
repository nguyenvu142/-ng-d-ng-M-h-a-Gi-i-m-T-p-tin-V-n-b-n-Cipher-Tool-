Cách 1: đứng trong thư mục tests (khớp cấu trúc trên repo)

Lệnh dịch: gcc gen_data.c -o gen_data

Cần có: thư mục data và out nằm sẵn bên trong tests.

Cách 2: đứng ở thư mục gốc (cách bạn đang dùng trên máy)

Lệnh dịch: gcc tests\gen_data.c -o gen_data

Cần có: thư mục data và out nằm ở thư mục gốc, ngang hàng với tests.

Các đường dẫn data/... và out/... trong code được tính từ thư mục bạn đang đứng khi chạy, nên chạy từ đâu thì hai thư mục này phải nằm ngay ở đó. Chương trình cipher.exe cũng phải nằm cùng chỗ với nơi bạn chạy run_tests.
