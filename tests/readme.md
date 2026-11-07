# Bộ kiểm thử (thư mục `tests`)

Phụ trách: Minh (E). Kiểm tra thuật toán mã hóa của nhóm bằng cách chạy tự động và so sánh từng byte.
Tất cả lệnh dưới đây chạy **từ thư mục gốc của repo** (nơi có `docs`, `tests`, `Makefile`).

## Các file

| File | Vai trò |
|---|---|
| `gen_data.c` | Tạo 12 tệp dữ liệu test trong `data/` (0 B, 1 B, 7/8/9 B, 256 giá trị byte, 1 KB, 65535/65536/65537 B quanh `BLOCK_SIZE`, tiếng Việt UTF-8, 100 MB). Không commit tệp lớn, chỉ commit script này |
| `run_tests.c` | Chạy chương trình của nhóm qua menu bằng tệp trả lời `ans.tmp`: mã hóa rồi giải mã, đáp án đã biết, khóa Caesar 1 đến 255, khóa dài, test âm tính. Menu Huy (mặc định) hoặc menu Vũ (`-DMENU_VU`). Kết quả: `ketqua.txt` |
| `test_modules.c` | Test mức hàm cho XOR, hoán vị, padding. Kết quả: `ketqua_modules.txt` |
| `test_crc32.c` | Test CRC32 |
| `test_common.h` | Mã lỗi `ERR_*`, `AlgoType`, `OpMode`, `BLOCK_SIZE` dùng chung cho các file test |
| `chay_test.sh`, `chay_test.bat` | Chạy cả bộ bằng một lệnh (Ubuntu/WSL và Windows) |
| `attack_caesar.c`, `attack_xor.c` | Demo bẻ Caesar (brute force) và XOR (known-plaintext) |

## Chạy cả bộ bằng một lệnh

Ubuntu/WSL: `sh chay_test.sh`   |   Windows: `chay_test.bat`

Mở script, mục `SRC` chọn chương trình cần test. Mặc định là bản ghép (`docs\main.c` + `docs\validate.c`
đã sửa khóa Caesar, cùng `caesar.c`, `tables.c`, `vigenere.c` ở thư mục gốc).

## Chạy từng phần (Windows; Ubuntu đổi `\` thành `/`, bỏ `.exe`)

```
mkdir data
mkdir out
mkdir obj
gcc -Wall -Wextra -std=c11 -pedantic -I. docs\main.c docs\validate.c caesar.c tables.c vigenere.c -o cipher.exe 2> canhbao.txt
gcc -Wall -Wextra -std=c11 -pedantic tests\gen_data.c -o gen_data
gen_data
gcc -Wall -Wextra -std=c11 -pedantic tests\run_tests.c -o run_tests
run_tests
```

Kết quả mong đợi: `canhbao.txt` trống, dòng cuối `=== PASS=69  FAIL=0 ===`.
Ubuntu: dịch `run_tests` thêm `-DCIPHER_CMD='"./cipher"'`.

## Test XOR, hoán vị, padding (mức hàm)

```
gcc -c -Dmain=xor_demo_main -Itests -Idocs docs/xorcipher.c -o obj/xor.o
gcc -c -Dmain=padding_demo_main -Itests tests/padding.c -o obj/padding.o
gcc -c -Dmain=transpose_demo_main -Itests tests/transpose_process.c -o obj/transpose.o
gcc -c -Idocs -Itests tests/permutation_module.c -o obj/perm.o
gcc -Wall -Wextra -std=c11 -pedantic -Idocs -Itests tests/test_modules.c obj/xor.o obj/padding.o obj/transpose.o obj/perm.o -o test_modules
test_modules
```

Chỉ xem phần XOR: Windows `test_modules | findstr xor`, Ubuntu `./test_modules | grep xor`.
`-Dmain=...` đổi tên `main` trong các file có `main()` riêng để ghép chung được.

## Test CRC32

```
gcc -Wall -Wextra -std=c11 -pedantic -Isrc -I. tests\test_crc32.c crc32.c -o test_crc32
test_crc32
```

## Test menu của Vũ (`src/main.c`, có XOR)

`src/main.c` dịch được riêng (không cần `adapter.c`), nhưng **chưa dùng thuật toán thật** của Huy và Tuấn: nó tự định nghĩa
hàm dự phòng `caesar_process`, `vigenere_process`, `xor_process` ngay trong file (Vigenère cộng byte khóa thay vì bảng tra A-Z,
XOR không xoay bit, hoán vị mục 4 chỉ đổi chỗ từng cặp byte). Dịch ra `cipher.exe` rồi chạy bản `run_tests` dành cho menu Vũ:

```
gcc -Wall -Wextra -std=c11 -pedantic -I. -Isrc src\main.c -o cipher.exe
gcc -Wall -Wextra -std=c11 -pedantic -DMENU_VU tests\run_tests.c -o run_tests_vu
run_tests_vu
```

Ubuntu: dịch `run_tests_vu` thêm `-DCIPHER_CMD='"./cipher"'`. `make` hiện còn lỗi (xem phần giới hạn).

## Kiểm tra chất lượng trong Ubuntu (WSL)

```
gcc -g -fsanitize=address,undefined -std=c11 -I. docs/main.c docs/validate.c caesar.c tables.c vigenere.c -o cipher_asan
gcc -std=c11 -DCIPHER_CMD='"./cipher_asan"' tests/run_tests.c -o run_tests_asan && ./run_tests_asan
gcc -g -std=c11 -I. docs/main.c docs/validate.c caesar.c tables.c vigenere.c -o cipher
printf '1\n1\n1\ndata/8192bit.bin\nout/v.tmp\n3\n0\n' > ans.tmp
valgrind --leak-check=full ./cipher < ans.tmp
cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem -I. docs/main.c docs/validate.c caesar.c tables.c vigenere.c 2> cppcheck.txt
```

## Kết quả hiện tại và giới hạn

| Bộ test | Kết quả |
|---|---|
| `run_tests` menu Huy (bản ghép) | 69 PASS, 0 FAIL |
| `run_tests` menu Huy (Caesar/Vigenère chính thức ở thư mục gốc, chưa có phần sửa khóa) | 62 PASS, 7 FAIL |
| `run_tests -DMENU_VU` (menu Vũ) | 44 PASS, 8 FAIL (khóa Caesar ngoài 1-255 và khóa quá dài: `atoi` không kiểm tra, `fgets` cắt khóa) |
| `test_modules` | 132 PASS, 0 FAIL |
| `test_crc32` | 4/4 |

- `make` hỏng: `src/adapter.c` cần `padding.h` nhưng `Makefile` thiếu `-Itests`; `adapter.c` gọi `get_inverse_permutation` đã bị gỡ khỏi `permutation_module.c`
  và gọi `pkcs7_unpad` theo chữ ký cũ (3 tham số, trả `int`) trong khi `padding.c` hiện là `void pkcs7_unpad(input, len, output, out_len)`.
- `tests/padding.h` khai báo khác `padding.c` nên `test_modules.c` khai báo trực tiếp `pkcs7_pad`, `pkcs7_unpad` theo `padding.c`.
- `test_modules.c` dùng `transpose_block` / `inverse_transpose_block` và tự tính hoán vị nghịch đảo, vì `apply_permutation` và `get_inverse_permutation` không còn trong `permutation_module.c`.
- Hoán vị chưa vào menu nào nên chưa có trong `run_tests`.
- Menu Huy chỉ nhận khóa Vigenère là chữ cái A–Z; menu Vũ nhận khóa tùy ý.
- Các thư mục `data/`, `out/`, `obj/` và tệp `*.exe`, `*.tmp`, `ketqua*.txt` sinh ra khi chạy, không commit.
