#include "validate.h"
#include <stdio.h>
#include <ctype.h>

bool validate_caesar_key(int key, bool byte_mode) {
    int modulus = byte_mode ? 256 : 26;
    int normalized = key % modulus;
    if (normalized < 0) normalized += modulus;

    if (normalized == 0) {
        printf("Canh bao: khoa Caesar khong lam thay doi du lieu.\n");
    }
    /* A no-op key is valid, but the user is warned. */
    return true;
}

bool validate_vigenere_key(const char *key) {
    if (key == NULL || key[0] == '\0') {
        fprintf(stderr, "Loi: khoa Vigenere khong duoc rong.\n");
        return false;
    }

    for (size_t i = 0; key[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)key[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            fprintf(stderr, "Loi: khoa Vigenere chi duoc chua chu cai A-Z.\n");
            return false;
        }
    }
    return true;
}
