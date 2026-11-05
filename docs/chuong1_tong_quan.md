# CHƯƠNG 1: TỔNG QUAN ĐỀ TÀI VÀ KIẾN TRÚC HỆ THỐNG

## 1.1. Đặt vấn đề và Mục tiêu đề tài
Trong bối cảnh lưu trữ và truyền tải dữ liệu số, việc bảo vệ tính bí mật và toàn vẹn của tệp tin là yêu cầu thiết yếu. Đề tài 15 — Ứng dụng Mã hóa & Giải mã Tệp tin (CipherTool) xây dựng một công cụ dòng lệnh (CLI) bằng ngôn ngữ C (chuẩn C99) cho phép xử lý trực tiếp trên luồng byte nhị phân (rb/wb) theo từng khối 64 KB (65536 bytes), đảm bảo xử lý mượt mà cả tệp văn bản lẫn tệp nhị phân dung lượng lớn mà không gây tràn bộ nhớ RAM.

## 1.2. Phân rã Module và Kiến trúc Hệ thống (System Map)
Hệ thống được thiết kế theo kiến trúc phân tầng mô-đun hóa, kết nối thông qua tệp hợp đồng giao diện cipher.h và common.h:
1. Tầng Giao diện & Điều phối (src/main.c, src/keyutil.c - Vũ phụ trách): Tiếp nhận lựa chọn từ người dùng, kiểm tra tính hợp lệ của tham số đường dẫn, khóa và điều phối luồng chạy thông qua Bảng con trỏ hàm (Dispatch Table).
2. Tầng Quản lý Luồng Tệp & Header (src/header.c, src/fileinfo.h - Vương phụ trách): Đọc/ghi tệp nhị phân theo khối 64 KB, trích xuất kích thước tệp (ThongTinTep) và quản lý cấu trúc Header nhận diện tệp mã hóa.
3. Tầng Thuật toán Cổ điển (caesar.c, vigenere.c, tables.c - Huy phụ trách): Thực thi phép dịch vòng modulo 256 (Caesar) và cộng khóa chuỗi tuần hoàn (Vigenère) trực tiếp trên mảng byte uint8_t.
4. Tầng Thuật toán Bitwise & Khối (xorcipher.c, permutation_module.h - Tuấn phụ trách): Thực thi phép toán XOR (^) theo chu kỳ khóa và thuật toán Hoán vị khối (Transposition) kết hợp cơ chế thêm/gỡ đệm (Padding).
5. Tầng Kiểm thử Tự động (tests/run_tests.c, tests/gen_data.c - Minh phụ trách): Sinh dữ liệu kiểm thử tự động và đối chiếu tính toàn vẹn đầu ra.

## 1.3. Thiết kế Bảng con trỏ hàm (Function Pointer Table) và Cơ chế Liên kết
Để tránh cấu trúc rẽ nhánh switch-case phức tạp và giúp chương trình dễ dàng mở rộng thuật toán mới, src/main.c định nghĩa kiểu con trỏ hàm đồng nhất ThuatToanFunc cùng các hàm bọc chuyển đổi kiểu khóa (Adapter Pattern):
- boc_caesar: Chuyển chuỗi khóa sang số dịch vòng int và gọi caesar_process().
- boc_vigenere: Truyền chuỗi khóa trực tiếp vào vigenere_process().
- boc_xor: Truyền mảng byte khóa và độ dài khóa vào xor_process().
- boc_transpose: Thực thi hoán vị khối dữ liệu cho Mốc 4.

Mảng cấu trúc BANG_CON_TRO_HAM[] ánh xạ trực tiếp mã số lựa chọn trên Menu (1..4) tới địa chỉ hàm tương ứng, giúp hàm chay_luong_tep() đọc từng khối 64 KB và gọi thẳng con trỏ hàm fn(buf, n_read, key, mode) với độ trễ bằng không.
