#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitops.h"

/*
 Áp dung: XOR với khóa tuần hoàn và phép xoay bit  
 Ý nghĩa: chống phương pháp phân tích tần suất các từ để dịch văn bản
 */
void xor_encrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return;
    }

    for (size_t i = 0; i < length; i++) {
        // Bước 1: XOR dữ liệu gốc với key tuần hoàn
        uint8_t xored = input[i] ^ key[i % key_len];
        // Bước 2: Xoay bit trái (ví dụ xoay 3 bit) để tăng tính xáo trộn
        output[i] = rotate_left8(xored, 3);
    }
}

/**
 * Giải mã mảng dữ liệu bằng cách xoay bit phải ngược lại và XOR
 */
void xor_decrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return;
    }

    for (size_t i = 0; i < length; i++) {
        // Bước 1: Xoay bit phải ngược lại 3 bit để trả về trạng thái sau XOR
        uint8_t unrotated = rotate_right8(input[i], 3);
        // Bước 2: XOR lại với key để lấy lại dữ liệu gốc
        output[i] = unrotated ^ key[i % key_len];
    }
}
