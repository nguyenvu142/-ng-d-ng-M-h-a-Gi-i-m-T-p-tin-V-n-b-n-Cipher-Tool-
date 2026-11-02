#ifndef CIPHER_H
#define CIPHER_H

#include <stddef.h> // Hỗ trợ kiểu size_t cho độ dài mảng
#include <stdint.h> // Hỗ trợ kiểu uint8_t cho từng byte dữ liệu

/* =========================================================================
 * QUY ĐỊNH CHUNG:
 * - mode = 1: Mã hóa (Encrypt)
 * - mode = 0: Giải mã (Decrypt)
 * - data: con trỏ trỏ tới mảng byte cần xử lý (sẽ bị ghi đè trực tiếp)
 * - length: số lượng byte cần xử lý
 * ========================================================================= */

// ---------------------------------------------------------
// 1. THUẬT TOÁN CAESAR & VIGENÈRE (Huy phụ trách)
// ---------------------------------------------------------

// Mã hóa/Giải mã bằng Caesar (dịch vòng)
void caesar_process(uint8_t *data, size_t length, int shift_key, int mode);

// Mã hóa/Giải mã bằng Vigenère (bảng tra)
void vigenere_process(uint8_t *data, size_t length, const char *text_key, int mode);

// ---------------------------------------------------------
// 2. THUẬT TOÁN XOR & PADDING (Tuấn phụ trách)
// ---------------------------------------------------------

// Xử lý XOR (Mã hóa và giải mã XOR bản chất giống nhau nên có thể không cần tham số mode)
void xor_process(uint8_t *data, size_t length, const uint8_t *key, size_t key_len);

// Hàm thêm đệm (Padding) cho dữ liệu không chia hết cho block_size
// Trả về kích thước mới sau khi đã thêm padding
size_t apply_padding(uint8_t *buffer, size_t current_len, size_t block_size);

// Hàm gỡ đệm (Remove Padding) sau khi giải mã
// Trả về kích thước thực tế của dữ liệu sau khi cắt bỏ padding
size_t remove_padding(const uint8_t *buffer, size_t padded_len);

// ---------------------------------------------------------
// 3. THUẬT TOÁN BỔ SUNG (Nếu cần - CRC32 của Huy)
// ---------------------------------------------------------
// Tính mã băm CRC32 để kiểm tra toàn vẹn tệp
uint32_t calculate_crc32(const uint8_t *data, size_t length);

#endif // CIPHER_H
