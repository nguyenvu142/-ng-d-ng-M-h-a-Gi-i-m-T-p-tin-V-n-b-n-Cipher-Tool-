# SƠ ĐỒ LUỒNG DỮ LIỆU HỆ THỐNG (SYSTEM MAP)
## 1. Luồng Mã hóa
[Tệp gốc (rb)] -> Đọc block 64 KB -> Tính CRC32 -> Thêm PKCS#7 Padding -> Gọi hàm thuật toán -> Ghi Header + Dữ liệu ra .tmp -> Đổi tên thành tệp đích.
## 2. Luồng Giải mã
[Tệp mã hóa (rb)] -> Kiểm tra Magic Header -> Giải mã block 64 KB -> Tháo Padding -> Đối chiếu CRC32 -> Khớp: đổi tên .tmp / Lệch: xóa .tmp và báo lỗi.
