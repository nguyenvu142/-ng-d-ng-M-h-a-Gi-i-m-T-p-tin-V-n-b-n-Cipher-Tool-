#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitops.h"

/**
 * Ý NGHĨA CỦA FILE NÀY: CHỐNG LẠI CÁC CUỘC PHÂN TÍCH TẦN SUẤT
 */
void xor_encrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, size_t key_len) {
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
    for (size_t i = 0; i < length; i++) {
        // Bước 1: Xoay bit phải ngược lại 3 bit để trả về trạng thái sau XOR
        uint8_t unrotated = rotate_right8(input[i], 3);
        // Bước 2: XOR lại với key để lấy lại dữ liệu gốc
        output[i] = unrotated ^ key[i % key_len];
    }
}

// Hàm main dùng để test độc lập (có thể không cần)
int main() {
    const char *plaintext = "Chao ban! Sinh vien UTC2 dang lam bai tap lon.";
    size_t len = strlen(plaintext);
    
    const uint8_t key[] = "UTC2_SECRET_KEY";
    size_t key_len = sizeof(key) - 1;

    uint8_t *encrypted = (uint8_t *)malloc(len);
    uint8_t *decrypted = (uint8_t *)malloc(len + 1);

    if (encrypted == NULL || decrypted == NULL) {
        printf("Loi cap phat bo nho!\n");
        return 1;
    }

    // 1. Tiến hành mã hóa
    xor_encrypt((const uint8_t *)plaintext, encrypted, len, key, key_len);
    printf("Du lieu sau ma hoa (Hex): ");
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", encrypted[i]);
    }
    printf("\n");

    // 2. Tiến hành giải mã
    xor_decrypt(encrypted, decrypted, len, key, key_len);
    decrypted[len] = '\0'; // Kết thúc chuỗi C-string
    printf("Du lieu sau giai ma: %s\n", decrypted);

    free(encrypted);
    free(decrypted);
    return 0;
}
