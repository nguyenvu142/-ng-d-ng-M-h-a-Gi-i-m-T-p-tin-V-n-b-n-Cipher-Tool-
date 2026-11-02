1. PHÉP TOÁN XOR
- Kí hiệu: *Chữ O có dấu cộng ở 
- Là phép toán logic cốt lỗi trong mật mã học dùng để xáo trộn dữ liệu thô và khóa
- Phép toán này so sánh dữ liệu gốc với từng bit bảo mật.
- Quy tắc: giống nhau = 0, khác nhau = 1, đặc biệt XOR 2 lần là cho ra kq gốc
vd: 0 XOR 0 = 0  ;  1 XOR 1 = 0  ;  0 XOR 1 = 1  ;  1 XOR 0 = 1;
Ngoài ra
A XOR B XOR B = 
Bản gốc: Plaintext ; Khóa: Key
Plaintext XOR Key = Ciphertext
Ciphertext XOR Key = Plaintext

2. HOÁN VỊ THEO TỪ KHÓA
- Là kỹ thuật xáo trộn trật tự vị trí của các ký tự trong
văn bản dựa trên thứ tự sắp xếp theo bảng chữ cái của Keyword
- Nếu chỉ đơn giản là thay từ này bằng từ khác
Ví dụ từ A thành B; B thành A
Thủ thuật này sẽ dễ dàng bị khắc chế bởi một thuật toán bằng cách
đếm tần suất xuất hiện phổ biển của kí tự, giúp kẻ xấu giải được
văn bản
=> Cho nên Hoán vị theo từ khóa sẽ giúp giải quyết vấn đề

3. PADDING
- Là quá trình thêm các byte/ký tự vô nghĩa (hoặc có quy luật định sẵn)
vào cuối văn bản gốc khi văn bản đó không đủ độ dài để chia hết cho kích thước
khối (Block size) của thuật toán.

- Quy ước: Số byte cần đệm cũng chính là giá trị của byte được đệm vào.
+ Ví dụ: Khối yêu cầu 8 byte. Dữ liệu chỉ có 5 byte (thiếu 3 byte). 
  Ta sẽ đệm thêm 3 byte có giá trị là 03. Chuỗi đệm sẽ là ... 03 03 03.
+ Nếu dữ liệu vừa khít kích thước khối, thuật toán vẫn sẽ
đệm thêm một khối hoàn chỉnh mới (ví dụ 8 byte toàn 08)
để bên nhận biết chắc chắn đâu là phần đệm khi giải mã.
- Vai trò: Bắt buộc phải có khi chuyển đổi mô hình mã hóa từ dạng Dòng (Stream Cipher)
- sang dạng Khối (Block Cipher) để đảm bảo mọi khối dữ liệu đi
- vào hàm XOR hay Hoán vị đều có kích thước bằng nhau


4. Ví dụ tay
Block Size: 4 bytes 
Plaintext): ANTOAN (6 ký tự).
Keyword): KEY (3 ký tự).
XOR Key): 01010101 

BƯỚC 1: XỬ LÝ PADDING 
6 chia 4 = 1 dư 2
Số byte còn thiếu để hoàn thành khối thứ hai là: 4 - 2 = 2 bytes.
Thiếu 2 bytes thì ta đệm thêm 2 ký tự có giá trị là 02 
Văn bản sau khi Padding: ANTOAN + [02][02]
=> Lúc này tổng độ dài là 8 ký tự, chia hết cho kích thước khối là 4)


BƯỚC 2: HOÁN VỊ TỪ KHÓA 
Dữ liệu 8 ký tự được chia thành 2 khối, mỗi khối 4 ký tự. 
Tuy nhiên, từ khóa KEY chỉ có 3 ký tự. 
Thuật toán sẽ dựa vào từ khóa để xáo trộn ma trận theo cột.
Đầu tiên, xác định thứ tự các cột dựa vào từ khóa KEY theo bảng chữ cái:
1. Ký tự E đứng đầu => Cột 1
2. Ký tự K đứng sau =>  Cột 2
3. Ký tự Y đứng cuối =>  Cột 
Xếp toàn bộ dữ liệu ANTOAN[02][02] vào bảng gồm 3 cột (theo độ dài của từ khóa):

Cột 2 (K)	Cột 1 (E)	Cột 3 (Y)
A	        N	        T
O	        A	        N
[02]	    [02]	    Trống

Đọc lại dữ liệu theo thứ tự ưu tiên của Cột (Cột 1 => Cột 2 => Cột 3):
• Cột 1 (E): N, A, [02] => NA[02]
• Cột 2 (K): A, O, [02] => AO[02]
• Cột 3 (Y): T, N => TN
Văn bản sau khi Hoán vị: NA[02]AO[02]TN (8 bytes).

BƯỚC 3: PHÉP TOÁN XOR 
Thuật toán sẽ quét qua từng ký tự của chuỗi NA[02]AO[02]TN 
và thực hiện phép toán XOR với Khóa 01010101.
Hãy thử nghiệm với ký tự đầu tiên là N:
• Ký tự: N
• Mã ASCII (Hệ 10): 78
• Mã Nhị phân (8-bit): 01001110
Thực hiện phép tính XOR:
text
   01001110  (Ký tự 'N')
XOR 01010101  (Khóa XOR)


Ký tự thứ hai: A
Mã ASCII (Hệ 10): 65
Mã Nhị phân: 01000001

Ký tự thứ tư: O
Mã ASCII (Hệ 10): 79
Mã Nhị phân: 01001111

Ký tự thứ năm: T
Mã ASCII (Hệ 10): 84
Mã Nhị phân: 01010100


Ký tự thứ sáu: N
Mã ASCII (Hệ 10): 78
Mã Nhị phân: 01001110


Ký tự thứ bảy (Byte Padding đầu tiên): x02
• Mã ASCII (Hệ 10): 2
• Mã Nhị phân: 00000010

























