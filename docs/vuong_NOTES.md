# vuong_NOTES – Phần D: Tệp và an toàn

## 1. struct ThongTinTep (fileinfo.h)

    path      char[512]   đường dẫn đầy đủ
    name      char[256]   tên tệp (đã tách khỏi đường dẫn)
    size      uint64_t    kích thước 64 bit
    exists    int         1 nếu tồn tại
    readable  int         1 nếu mở đọc được

ttt_load(path, out): dùng _stati64 trên Windows, stat trên hệ khác.
Trả về ERR_NOT_FOUND, ERR_IS_DIR, ERR_NO_READ hoặc ERR_OK.

## 2. Bố cục header tệp mã hóa (cố định 32 byte, little-endian)

    Offset  Độ dài  Trường
    0       4       dấu nhận diện "CTL1"
    4       1       phiên bản (1)
    5       1       mã thuật toán (1 Caesar, 2 Vigenère, 3 XOR, 4 hoán vị)
    6       2       flags (bit 0 = có padding, bit 1 = chế độ chữ cái)
    8       4       blockSize N (mặc định 8, cho phép 16)
    12      8       kích thước gốc
    20      4       CRC32 của dữ liệu gốc
    24      8       dự trữ (0)

Dữ liệu đã mã hóa bắt đầu từ byte 32.

## 3. Pseudocode process_file (mã hóa)

    process_file(in, out, alg, flags, N, key, fn):
        kiểm tra key không rỗng, N hợp lệ (N chia hết BLOCK_SIZE)
        ttt_load(in) -> lỗi nếu không tồn tại / không đọc được
        nếu in == out -> ERR_SAME_FILE
        mở in "rb", mở out.tmp "wb", cấp phát buf một lần
        ghi header (CRC tạm = 0)
        offset = 0, crc = 0
        lặp:
            n = đọc tối đa BLOCK_SIZE byte vào buf
            nếu n < BLOCK_SIZE: đây là khối cuối
            crc = crc32_update(crc, buf, n)
            nếu là khối cuối và có padding: n = pad_block(buf, n, N)
            fn(buf, n, key, offset, ENCRYPT)
            ghi buf (n byte)
            offset += số byte đã đọc
            hiện % tiến độ
        quay lại đầu tệp, ghi lại header với CRC thật
        đóng tệp; thành công -> đổi tên out.tmp thành out
                  thất bại   -> xóa out.tmp

## 4. Pseudocode decrypt_file

    đọc header -> sai dấu nhận diện / phiên bản thì báo lỗi
    tra fn từ mã thuật toán trong header
    lặp từng khối: fn(buf, n, key, offset, DECRYPT)
        ở khối cuối nếu có padding: unpad_block (sai -> "sai khóa hoặc tệp hỏng")
        cập nhật CRC, ghi ra out.tmp
    nếu CRC hoặc kích thước khác header: xóa out.tmp, báo "sai khóa hoặc tệp hỏng"

## 5. Stub identity_process (stubs.c)

Hàm giả: không đổi buf, chỉ trả ERR_OK (ERR_EMPTY_KEY nếu khóa rỗng).
Dùng để chạy thử luồng tệp trước khi Huy và Tuấn nộp bản thật.
