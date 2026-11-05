# CipherTool - Ứng dụng Mã hóa & Giải mã Tệp tin (Đề tài 15)

Chương trình dòng lệnh (CLI) viết bằng ngôn ngữ C (chuẩn C99) hỗ trợ mã hóa và giải mã tệp tin nhị phân theo khối 64 KB với 4 thuật toán: **Caesar, Vigenère, XOR và Hoán vị khối (Transposition)**.

## 1. Yêu cầu hệ thống
- Trình biên dịch: gcc (hỗ trợ chuẩn C99 trở lên) và công cụ make.
- Môi trường: Linux, macOS, Windows (Git Bash / WSL / MinGW) hoặc GitHub Codespaces.

## 2. Hướng dẫn biên dịch (Build)
Mở Terminal tại thư mục gốc của dự án và chạy các lệnh sau:
- Biên dịch bản tiêu chuẩn: `make clean && make all`
- Biên dịch bản gỡ lỗi (AddressSanitizer - ASan): `make debug`
- Dọn dẹp các tệp đối tượng (.o) và tệp thực thi: `make clean`

## 3. Hướng dẫn chạy chương trình và Kiểm thử
- Chạy chương trình chính (Menu tương tác): `./cipher_tool`
- Kiểm thử nhanh luồng mã hóa/giải mã và đối chiếu toàn vẹn bằng cmp:
  1. Tạo tệp mẫu: `echo "Du lieu kiem thu CipherTool" > sample.txt`
  2. Mã hóa: `printf "1\n1\nsample.txt\nsample.enc\n5\n0\n" | ./cipher_tool`
  3. Giải mã: `printf "1\n0\nsample.enc\nsample.dec\n5\n0\n" | ./cipher_tool`
  4. So sánh: `cmp sample.txt sample.dec`
