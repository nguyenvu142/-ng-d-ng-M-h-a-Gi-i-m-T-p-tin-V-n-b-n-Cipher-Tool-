#include "transpose_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void transpose_block(const uint8_t *input, uint8_t *output, const uint8_t *perm) {
    if (input == NULL || output == NULL || perm == NULL) return;
    for (size_t i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) {
        output[i] = input[perm[i]];
    }
}

void inverse_transpose_block(const uint8_t *input, uint8_t *output, const uint8_t *inv_perm) {
    if (input == NULL || output == NULL || inv_perm == NULL) return;
    for (size_t i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) {
        output[i] = input[inv_perm[i]];
    }
}

int main() {
    uint8_t input_block[TRANSPOSE_BLOCK_SIZE] = {'U', 'T', 'C', '2', 'C', 'O', 'D', 'E'};
    uint8_t output_block[TRANSPOSE_BLOCK_SIZE];
    uint8_t restored_block[TRANSPOSE_BLOCK_SIZE];

    uint8_t mock_perm[TRANSPOSE_BLOCK_SIZE] = {3, 0, 4, 1, 7, 2, 5, 6};
    uint8_t mock_inv_perm[TRANSPOSE_BLOCK_SIZE];

    for (size_t i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) {
        mock_inv_perm[mock_perm[i]] = (uint8_t)i;
    }

    printf("[>] Original: ");
    for (int i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) printf("%c ", input_block[i]);
    printf("\n");

    transpose_block(input_block, output_block, mock_perm);
    printf("[+] Transposed: ");
    for (int i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) printf("%c ", output_block[i]);
    printf("\n");

    inverse_transpose_block(output_block, restored_block, mock_inv_perm);
    printf("[<] Restored: ");
    for (int i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) printf("%c ", restored_block[i]);
    printf("\n");

    int success = 1;
    for (int i = 0; i < TRANSPOSE_BLOCK_SIZE; i++) {
        if (input_block[i] != restored_block[i]) {
            success = 0;
            break;
        }
    }

    if (success) {
        printf("[SUCCESS]\n");
    } else {
        printf("[FAILED]\n");
    }

    return 0;
}
