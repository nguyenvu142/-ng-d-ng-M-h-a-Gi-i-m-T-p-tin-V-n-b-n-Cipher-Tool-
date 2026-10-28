# Huy NOTES - Caesar & Vigenere
<img width="616" height="375" alt="image" src="https://github.com/user-attachments/assets/178e0ee7-4cd9-49fa-a13f-58031d3c4354" />

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

## 2. Vigenère Cipher
-dùng để biến văn bản gốc thành văn bản đã mã hóa bằng một chuỗi khóa.
-quy định bảng chữ cái từ A-Z sẽ thành số từ 0-25 (A=0; B=1; ...)

-công thức nhuw sau:
- kí tự sau khi mã hóa = ( kí tự gốc + kí tự sau khi khóa ) chia cho 26 lấy dư
- ví dụ tôi muốn mã hóa chữ "hello"
- thì tôi chọn một từ khóa nào đó để khóa ví dụ "key"
- mà key chỉ có 3 chữ cái bé hơn hello 5 chữ cái 
- nên kí tự khóa sẽ là keyke ( nếu mà chữ mã hóa là 6 hay 7,8 chữ cái chẳng hạn thì dễ thấy ta cứ tiếp tục lặp lại keykeykeykey thôi
-trường hợp này là keyke
- bắt đầu mã hóa như sau
- -+hello
-chữ cái h đứng thứ 7 nên tôi viết gọn h=7 -tương tự e=4, l=11,...
- +keyke cũng tương tự 
-k=10, e=4, ...
-sau đó ta cộng số đã tính theo vị trí h-k, e-e, l-y,...
-được từng cặp như sau 17, 8, 35, 21, 18
-ta tiếp tục thay các số đó ngược lại thành chữ sau khi chia 26 lấy dư là được một chuỗi mã hóa rồi
- 17 % 26= 17 ->R
- 8 % 26= 8 ->I
- 35 % 26= 9 -> J
- 21 % 26= 21-> V
- 18%26=18 -> S
- vậy mã hóa cho từ ''hello'' với khóa ''key'' là RIJVS
