# Sơ đồ Luồng Dữ Liệu Hệ Thống (System Data Flow)

Tài liệu này mô tả luồng dữ liệu 1 trang, từ lúc người dùng tương tác đến khi xuất ra tệp kết quả. Toàn bộ các module độc lập của nhóm sẽ được ghép nối theo thứ tự này.

## 1. Sơ đồ tổng quan (Pipeline)

```text
[Người dùng] 
    │
    ▼
(1) CLI / MENU (Vũ) 
    - Nhận tham số: Tên file IN, file OUT, Thuật toán, Chế độ (Mã hóa/Giải mã), Khóa (Key).
    │
    ▼
(2) TRÌNH QUẢN LÝ TỆP (Vương)
    - Đọc file IN theo từng khối byte (block) hoặc toàn bộ.
    - Đổ dữ liệu vào `struct ThongTinTep` (khai báo trong common.h).
    │
    ▼
(3) BỘ TIỀN XỬ LÝ (Tuấn) -- [Chỉ áp dụng nếu Mã hóa & cần chia block]
    - Gọi hàm `apply_padding()` để làm tròn dữ liệu.
    │
    ▼
(4) BỘ MÃ HÓA/GIẢI MÁ CỐT LÕI (Huy / Tuấn)
    - Dựa vào tham số thuật toán, gọi các hàm từ `cipher.h`:
      + `caesar_process()`
      + `vigenere_process()`
      + `xor_process()`
    - Dữ liệu bị biến đổi (ghi đè trực tiếp lên mảng byte).
    │
    ▼
(5) BỘ HẬU XỬ LÝ (Tuấn / Huy) -- [Chỉ áp dụng nếu Giải mã]
    - Gọi hàm `remove_padding()` để loại bỏ byte thừa.
    - Gọi `calculate_crc32()` để kiểm tra tính toàn vẹn (tùy chọn).
    │
    ▼
(6) XUẤT TỆP (Vương)
    - Ghi mảng byte đã xử lý ra file OUT.
    - Đóng tệp, dọn dẹp bộ nhớ.
