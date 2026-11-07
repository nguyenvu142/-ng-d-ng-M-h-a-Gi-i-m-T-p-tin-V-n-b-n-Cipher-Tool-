#include <stdio.h>
#include <string.h>
#include "keyutil.h"

void get_cipher_key(int algo_id, uint8_t *key_buffer, size_t *key_len) {
    if (!key_buffer || !key_len) return;
    char tmp[256];
    printf("Nhap khoa cho thuat toan %d: ", algo_id);
    if (fgets(tmp, sizeof(tmp), stdin)) {
        size_t n = strlen(tmp);
        while (n > 0 && (tmp[n - 1] == '\n' || tmp[n - 1] == '\r')) {
            tmp[--n] = '\0';
        }
        memcpy(key_buffer, tmp, n + 1);
        *key_len = n;
    } else {
        *key_len = 0;
    }
}
