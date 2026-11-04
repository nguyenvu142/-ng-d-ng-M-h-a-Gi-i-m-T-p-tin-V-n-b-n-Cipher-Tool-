#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "tables.h"
#include "caesar.h"
#include "vigenere.h"
#include "validate.h"

#define BUFFER_SIZE 4096

static int process_file(const char *input_path, const char *output_path,
                        int algorithm, int mode, int decrypt,
                        int caesar_key, const char *vig_key) {
    if (input_path == NULL || output_path == NULL ||
        strcmp(input_path, output_path) == 0) {
        fprintf(stderr, "Loi: tep dau vao va tep dau ra phai khac nhau.\n");
        return 0;
    }

    FILE *in = fopen(input_path, "rb");
    if (in == NULL) {
        perror("Khong mo duoc tep dau vao");
        return 0;
    }

    FILE *out = fopen(output_path, "wb");
    if (out == NULL) {
        perror("Khong tao duoc tep dau ra");
        fclose(in);
        return 0;
    }

    uint8_t buffer[BUFFER_SIZE];
    size_t n;
    size_t offset = 0;
    size_t key_len = vig_key != NULL ? strlen(vig_key) : 0;
    int ok = 1;

    while ((n = fread(buffer, 1, sizeof buffer, in)) > 0) {
        if (algorithm == 1) {
            if (mode == 1) {
                caesar_process_bytes(buffer, n, caesar_key, decrypt != 0);
            } else {
                caesar_process_letters(buffer, n, caesar_key, decrypt != 0);
            }
        } else if (algorithm == 2) {
            if (mode == 1) {
                vigenere_process_bytes(buffer, n, (const uint8_t *)vig_key,
                                       key_len, &offset, decrypt != 0);
            } else {
                vigenere_process_letters(buffer, n, vig_key, key_len,
                                         &offset, decrypt != 0);
            }
        } else {
            fprintf(stderr, "Loi: thuat toan khong hop le.\n");
            ok = 0;
            break;
        }

        if (fwrite(buffer, 1, n, out) != n) {
            perror("Loi ghi tep");
            ok = 0;
            break;
        }
    }

    if (ferror(in)) {
        perror("Loi doc tep");
        ok = 0;
    }

    if (fclose(in) != 0) {
        perror("Loi dong tep dau vao");
        ok = 0;
    }
    if (fclose(out) != 0) {
        perror("Loi dong tep dau ra");
        ok = 0;
    }

    if (!ok) {
        remove(output_path);
        return 0;
    }
    return 1;
}

int main(void) {
    int choice, mode, key, operation;
    char input[512], output[512], vig_key[256];

    tables_init();

    for (;;) {
        puts("\n=== CIPHER: MA HOA / GIAI MA TEP ===");
        puts("1. Caesar Cipher");
        puts("2. Vigenere Cipher");
        puts("0. Thoat");
        printf("Lua chon: ");

        if (scanf("%d", &choice) != 1) return 1;
        if (choice == 0) break;
        if (choice != 1 && choice != 2) {
            puts("Lua chon khong hop le.");
            continue;
        }

        printf("Che do (1=byte, 2=chu cai ASCII): ");
        if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
            puts("Che do khong hop le.");
            return 1;
        }

        printf("Tac vu (1=ma hoa, 2=giai ma): ");
        if (scanf("%d", &operation) != 1 ||
            (operation != 1 && operation != 2)) {
            puts("Tac vu khong hop le.");
            return 1;
        }
        int decrypt = (operation == 2);

        printf("Tep dau vao: ");
        if (scanf(" %511[^\n]", input) != 1) return 1;
        printf("Tep dau ra: ");
        if (scanf(" %511[^\n]", output) != 1) return 1;
        if (strcmp(input, output) == 0) {
            puts("Loi: tep dau vao va tep dau ra phai khac nhau.");
            continue;
        }

        if (choice == 1) {
            printf("Khoa so Caesar: ");
            if (scanf("%d", &key) != 1) return 1;
            if (!validate_caesar_key(key, mode == 1)) continue;

            if (process_file(input, output, choice, mode, decrypt, key, NULL)) {
                printf("Hoan tat: %s\n", output);
            }
        } else {
            printf("Khoa Vigenere (A-Z): ");
            if (scanf(" %255s", vig_key) != 1) return 1;
            if (!validate_vigenere_key(vig_key)) continue;

            for (size_t i = 0; vig_key[i] != '\0'; ++i) {
                if (vig_key[i] >= 'a' && vig_key[i] <= 'z') {
                    vig_key[i] = (char)(vig_key[i] - 'a' + 'A');
                }
            }

            if (process_file(input, output, choice, mode, decrypt, 0, vig_key)) {
                printf("Hoan tat: %s\n", output);
            }
        }
    }

    return 0;
}
