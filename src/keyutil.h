#ifndef KEYUTIL_H
#define KEYUTIL_H

#include <stdint.h>
#include <stddef.h>

// Hàm lấy khóa từ người dùng (Caesar dùng số, các thuật toán khác dùng chuỗi)
void get_cipher_key(int algo_id, uint8_t *key_buffer, size_t *key_len);

#endif