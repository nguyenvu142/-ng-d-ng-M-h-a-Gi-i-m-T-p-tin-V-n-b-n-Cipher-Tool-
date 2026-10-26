# Huy NOTES - Caesar & Vigenere

## 1. Caesar Cipher
Nó chỉ là:
Lấy chữ cái này dịch chuyển đi một số bước
Dịch đi một số bước (gọi là khóa)
Hết bảng chữ cái thì quay lại đầu
### Cách hoạt động
- Dịch mỗi ký tự đi một số vị trí cố định (gọi là khóa/shift).
- Ví dụ khóa = 3:(bước 3 bước)
  - A → D
  - B → E
  - ...
  - X → A
  - Y → B
  - Z → C

### Ví dụ tính tay 1: HELLO, khóa = 3
- H (7) + 3 = 10 → K
- E (4) + 3 = 7 → H
- L (11) + 3 = 14 → O
- L (11) + 3 = 14 → O
- O (14) + 3 = 17 → R

→ Kết quả: **KHOOR**
