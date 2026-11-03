#include "permutation_module.h"

void perm_from_key(const uint8_t *key, size_t key_len, uint8_t *perm) {
    if (key == NULL || key_len == 0 || perm == NULL) {
        return;
    }

    // Bước 1: Khởi tạo mảng hoán vị cơ sở có thứ tự tăng dần [0, 1, 2, ..., BLOCK_SIZE-1]
    for (size_t i = 0; i < PERM_MODULE_BLOCK_SIZE; i++) {
        perm[i] = (uint8_t)i;
    }

    // Bước 2: Tiến hành hoán vị các phần tử dựa trên giá trị của khóa (Key)
    size_t j = 0;
    for (size_t i = 0; i < PERM_MODULE_BLOCK_SIZE; i++) {
        // Thuật toán xáo trộn giả ngẫu nhiên có tính chu kỳ dựa trên Key
        j = (j + perm[i] + key[i % key_len]) % PERM_MODULE_BLOCK_SIZE;
        
        // Tráo đổi giá trị giữa vị trí i và vị trí j (Swap)
        uint8_t temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }
}

void get_inverse_permutation(const uint8_t *perm, uint8_t *inv_perm, size_t size) {
    if (perm == NULL || inv_perm == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
       
        inv_perm[perm[i]] = (uint8_t)i;
    }
}

void apply_permutation(const uint8_t *input, uint8_t *output, const uint8_t *perm, size_t size) {
    if (input == NULL || output == NULL || perm == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
        output[i] = input[perm[i]];
    }
}
