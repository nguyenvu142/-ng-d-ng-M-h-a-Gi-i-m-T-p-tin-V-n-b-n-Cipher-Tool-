[README.md](https://github.com/user-attachments/files/34091622/README.md)
# CIPHER - Bai tap Caesar va Vigenere (ngon ngu C)

## 1. Muc tieu
- Tao bang tra Caesar 256 phan tu va bang nguoc bang code.
- Tao bang Vigenere 26 x 26 bang code, khong viet tay.
- Ho tro Caesar theo byte (modulo 256) va theo chu cai ASCII.
- Ho tro Vigenere theo byte (dung `offset`) va theo chu cai (dung vi tri khoa).
- Kiem tra khoa va chay unit test.
- Ghi cong thuc, pseudocode tieng Viet, vi du tinh tay va vector kiem thu.
- Dung mot chuong trinh C doc lap de doi chieu ket qua, khong can Python.

## 2. Cau truc thu muc

```text
Cipher/
|-- main.c
|-- tables.c
|-- tables.h
|-- caesar.c
|-- caesar.h
|-- vigenere.c
|-- vigenere.h
|-- validate.c
|-- validate.h
|-- docs/
|   `-- huy_NOTES.md
`-- tests/
    |-- test_classic.c
    |-- reference_check.c
    `-- vectors.txt
```

## 3. Bien dich chuong trinh chinh

Chay lenh tai thu muc `Cipher`:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c tables.c caesar.c vigenere.c validate.c -o cipher
```

Windows:

```bash
cipher.exe
```

Linux/macOS:

```bash
./cipher
```

Trong menu, chon thuat toan, che do byte/chu cai, ma hoa/giai ma, tep dau vao, tep dau ra va khoa.

## 4. Chay unit test cho cac ham cua du an

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic tests/test_classic.c tables.c caesar.c vigenere.c validate.c -o test_classic
```

Windows: `test_classic.exe`

Linux/macOS: `./test_classic`

Ket qua thanh cong: `All tests passed.`

## 5. Kiem tra doc lap bang C

`tests/reference_check.c` tu cai dat lai cong thuc Caesar va Vigenere chu cai, khong goi cac ham xu ly cua du an. File nay dung de doi chieu ket qua voi unit test chinh.

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic tests/reference_check.c -o reference_check
```

Windows: `reference_check.exe`

Linux/macOS: `./reference_check`

Ket qua mong doi: `Ket qua: 7/7 vector dat.`

## 6. Vector mau

Cac vector nam trong `tests/vectors.txt`, gom:
- Caesar: `HELLO`, khoa 3 -> `KHOOR`.
- Caesar: `XYZ`, khoa 3 -> `ABC`.
- Vigenere: `ATTACKATDAWN`, khoa `LEMON` -> `LXFOPVEFRNHR`.
- Caesar byte: `0xFF`, khoa 1 -> `0x00`.
- Vigenere byte: `00 FF 10`, khoa byte `01 02` -> `01 01 11`.
- Khoa Caesar khong lam thay doi du lieu can duoc canh bao.
- Khoa Vigenere rong phai bi tu choi.

## 7. Ghi chu

- Che do chu cai chi bien doi chu ASCII A-Z/a-z; dau cach va dau cau duoc giu nguyen.
- Che do byte xu ly tung byte theo modulo 256; khong dong nghia voi ma hoa van ban tieng Viet theo ky tu Unicode.
- Caesar va Vigenere co dien chi phu hop hoc tap, khong dung de bao ve du lieu nhay cam trong thuc te.
