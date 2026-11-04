# Bộ kiểm thử (thư mục `tests`)

Phụ trách: Minh (E). Mục đích: kiểm tra các thuật toán mã hóa của nhóm bằng cách
chạy tự động và so sánh từng byte, không dùng dữ liệu thật của người dùng.

## Các file

| File | Vai trò |
|---|---|
| `gen_data.c` | Tạo 12 tệp dữ liệu test trong `data/` (0 B, 1 B, 7/8/9 B, 256 giá trị byte, 1 KB, 65535/65536/65537 B quanh `BLOCK_SIZE`, tiếng Việt UTF-8, 100 MB). Không commit tệp lớn, chỉ commit script này |
| `run_tests.c` | Chạy `cipher.exe` (menu của Huy) với 65 phép thử: mã hóa rồi giải mã, đáp án đã biết, khóa Caesar 1 đến 255, test âm tính. Kết quả ghi vào `ketqua.txt` |
| `test_modules.c` | Test mức hàm cho XOR, hoán vị, padding (các module này chưa vào menu). Kết quả ghi vào `ketqua_modules.txt` |
| `test_common.h` | Mã lỗi `ERR_*`, `AlgoType`, `OpMode`, `BLOCK_SIZE` dùng chung cho các file test |
| `attack_caesar.c` | Demo bẻ Caesar bằng brute force (thử mọi khóa) |
| `attack_xor.c` | Demo bẻ XOR bằng known-plaintext (khóa = bản mã XOR bản rõ) |

## Chuẩn bị (chạy từ thư mục gốc của repo)

```
mkdir data
mkdir out
gcc -Wall -Wextra -std=c11 -pedantic docs\main.c docs\tables.c docs\caesar.c docs\vigenere.c docs\validate.c -o cipher 2> canhbao.txt
gcc -Wall -Wextra -std=c11 -pedantic tests\gen_data.c -o gen_data
gen_data
```

`canhbao.txt` trống nghĩa là code dịch không có cảnh báo. Không dùng `docs/*.c` để dịch,
vì thư mục `docs` có nhiều tệp `main()` riêng của từng thành viên.

## Chạy test mã hóa (Caesar, Vigenère)

```
gcc -Wall -Wextra -std=c11 -pedantic tests\run_tests.c -o run_tests
run_tests
```

Kết quả mong đợi: dòng cuối `=== PASS=65  FAIL=0 ===`.
Nếu báo "khong chay duoc cipher.exe" thì chưa dịch `cipher.exe` hoặc đang đứng sai thư mục.

## Chạy test các module của Tuấn (XOR, hoán vị, padding)

```
mkdir obj
gcc -c -Dmain=xor_demo_main -Itests docs/xorcipher.c -o obj/xor.o
gcc -c -Dmain=padding_demo_main -Itests tests/padding.c -o obj/padding.o
gcc -c -Dmain=transpose_demo_main -Itests tests/transpose_process.c -o obj/transpose.o
gcc -c -Idocs -Itests tests/permutation_module.c -o obj/perm.o
gcc -Wall -Wextra -std=c11 -pedantic -Idocs -Itests tests/test_modules.c obj/xor.o obj/padding.o obj/transpose.o obj/perm.o -o test_modules
test_modules
```

`-Dmain=...` đổi tên `main` trong từng file của Tuấn để ghép chung được.

## Demo tấn công

```
gcc -Wall -Wextra -std=c11 -pedantic tests\attack_caesar.c -o attack_caesar
attack_caesar
```

## Giới hạn hiện tại

- `run_tests` mới kiểm thử Caesar và Vigenère. XOR và hoán vị sẽ được thêm khi `main.c` có hai thuật toán này.
- `run_tests` gọi chương trình qua menu bằng tệp trả lời `ans.tmp`. Nếu `main.c` chính chuyển sang tham số dòng lệnh thì chỉ cần sửa hàm `chay_cipher`.
- Các thư mục `data/`, `out/` và tệp `*.exe`, `*.tmp`, `ketqua*.txt` sinh ra khi chạy test, không commit.
