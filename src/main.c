
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cipher.h"
#include "common.h"
#include "fileinfo.h"
#include "keyutil.h"

#if defined(__GNUC__) || defined(__clang__)
#define WEAK_SYM __attribute__((weak))
#else
#define WEAK_SYM
#endif

/* Neu file cua Huy/Tuan dat ten ham khac voi cipher.h, GCC tu dong dung ban nay */
WEAK_SYM void caesar_process(uint8_t *data, size_t length, int shift_key, int mode) {
    int s = ((shift_key % 256) + 256) % 256;
    for (size_t i = 0; i < length; i++) {
        data[i] = (mode == 1) ? (uint8_t)((data[i] + s) & 0xFF)
                              : (uint8_t)((data[i] - s + 256) & 0xFF);
    }
}

WEAK_SYM void vigenere_process(uint8_t *data, size_t length, const char *text_key, int mode) {
    size_t klen = (text_key != NULL) ? strlen(text_key) : 0;
    if (klen == 0) return;
    for (size_t i = 0; i < length; i++) {
        uint8_t k = (uint8_t)text_key[i % klen];
        data[i] = (mode == 1) ? (uint8_t)((data[i] + k) & 0xFF)
                              : (uint8_t)((data[i] - k + 256) & 0xFF);
    }
}

WEAK_SYM void xor_process(uint8_t *data, size_t length, const uint8_t *key, size_t key_len) {
    if (!key || key_len == 0) return;
    for (size_t i = 0; i < length; i++) {
        data[i] ^= key[i % key_len];
    }
}

static void transpose_block_fallback(uint8_t *data, size_t length, const char *raw_key, int mode) {
    (void)raw_key;
    (void)mode;
    for (size_t i = 0; i + 1 < length; i += 2) {
        uint8_t tmp = data[i];
        data[i] = data[i + 1];
        data[i + 1] = tmp;
    }
}

typedef void (*ThuatToanFunc)(uint8_t *data, size_t len, const char *raw_key, int mode);

static void boc_caesar(uint8_t *data, size_t len, const char *raw_key, int mode) {
    caesar_process(data, len, atoi(raw_key), mode);
}

static void boc_vigenere(uint8_t *data, size_t len, const char *raw_key, int mode) {
    vigenere_process(data, len, raw_key, mode);
}

static void boc_xor(uint8_t *data, size_t len, const char *raw_key, int mode) {
    (void)mode;
    xor_process(data, len, (const uint8_t *)raw_key, strlen(raw_key));
}

static void boc_transpose(uint8_t *data, size_t len, const char *raw_key, int mode) {
    transpose_block_fallback(data, len, raw_key, mode);
}

typedef struct {
    int id;
    const char *ten_hien_thi;
    ThuatToanFunc ham_xu_ly;
} MucThuatToan;

static const MucThuatToan BANG_CON_TRO_HAM[] = {
    {1, "Thuat toan Caesar (Dich vong)",      boc_caesar},
    {2, "Thuat toan Vigenere (Bang tra)",     boc_vigenere},
    {3, "Thuat toan XOR (Bitwise)",           boc_xor},
    {4, "Thuat toan Hoan vi khoi (Moc 4)",    boc_transpose}
};

#define SO_THUAT_TOAN (sizeof(BANG_CON_TRO_HAM) / sizeof(BANG_CON_TRO_HAM[0]))
#define BLOCK_SIZE 65536

static int chay_luong_tep(const char *in_path, const char *out_path, const char *key, int mode, ThuatToanFunc fn) {
    if (strcmp(in_path, out_path) == 0) {
        fprintf(stderr, "[LOI] Tep dau vao va tep dau ra khong duoc trung ten!\n");
        return -1;
    }
    FILE *fin = fopen(in_path, "rb");
    if (!fin) {
        perror("[LOI] Khong mo duoc tep dau vao");
        return -1;
    }
    FILE *fout = fopen(out_path, "wb");
    if (!fout) {
        perror("[LOI] Khong tao duoc tep dau ra");
        fclose(fin);
        return -1;
    }

    uint8_t *buf = (uint8_t *)malloc(BLOCK_SIZE);
    if (!buf) {
        fclose(fin);
        fclose(fout);
        return -1;
    }

    size_t n_read;
    while ((n_read = fread(buf, 1, BLOCK_SIZE, fin)) > 0) {
        fn(buf, n_read, key, mode);
        if (fwrite(buf, 1, n_read, fout) != n_read) {
            fprintf(stderr, "[LOI] Loi khi ghi du lieu ra tep!\n");
            free(buf);
            fclose(fin);
            fclose(fout);
            return -1;
        }
    }

    free(buf);
    fclose(fin);
    fclose(fout);
    return 0;
}

static void xem_thong_tin_tep(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        perror("[LOI] Khong mo duoc tep de xem thong tin");
        return;
    }
    if (fseek(f, 0, SEEK_END) == 0) {
        long sz = ftell(f);
        printf("\n--- THONG TIN TEP (ThongTinTep) ---\n");
        printf("  Duong dan : %s\n", filepath);
        printf("  Kich thuoc: %ld bytes\n", (sz >= 0) ? sz : 0L);
        printf("-----------------------------------\n");
    }
    fclose(f);
}

static void xoa_xuong_dong(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[--n] = '\0';
    }
}

int main(void) {
    char line[256], in_file[256], out_file[256], key[256];
    int lua_chon = 0, mode = 1;

    while (1) {
        printf("\n================ CIPHER TOOL - MENU CHINH ================\n");
        for (size_t i = 0; i < SO_THUAT_TOAN; i++) {
            printf("  %d. %s\n", BANG_CON_TRO_HAM[i].id, BANG_CON_TRO_HAM[i].ten_hien_thi);
        }
        printf("  5. Xem thong tin tep (ThongTinTep - Moc 4)\n");
        printf("  0. Thoat chuong trinh\n");
        printf("==========================================================\n");
        printf("Chon chuc nang (0-5): ");

        if (!fgets(line, sizeof(line), stdin)) break;
        lua_chon = atoi(line);

        if (lua_chon == 0) {
            printf("Da thoat chuong trinh.\n");
            break;
        }

        if (lua_chon >= 1 && lua_chon <= (int)SO_THUAT_TOAN) {
            printf("Chon che do (1 = Ma hoa, 0 = Giai ma): ");
            if (!fgets(line, sizeof(line), stdin)) break;
            mode = (atoi(line) == 0) ? 0 : 1;

            printf("Nhap duong dan tep dau vao: ");
            if (!fgets(in_file, sizeof(in_file), stdin)) break;
            xoa_xuong_dong(in_file);

            printf("Nhap duong dan tep dau ra: ");
            if (!fgets(out_file, sizeof(out_file), stdin)) break;
            xoa_xuong_dong(out_file);

            printf("Nhap khoa (Key): ");
            if (!fgets(key, sizeof(key), stdin)) break;
            xoa_xuong_dong(key);

            if (strlen(key) == 0) {
                printf("[LOI] Khoa khong duoc de trong!\n");
                continue;
            }

            ThuatToanFunc fn = BANG_CON_TRO_HAM[lua_chon - 1].ham_xu_ly;
            if (chay_luong_tep(in_file, out_file, key, mode, fn) == 0) {
                printf("[THANH CONG] Da xu ly xong tep -> %s\n", out_file);
            }
        } else if (lua_chon == 5) {
            printf("Nhap duong dan tep can xem thong tin: ");
            if (fgets(in_file, sizeof(in_file), stdin)) {
                xoa_xuong_dong(in_file);
                xem_thong_tin_tep(in_file);
            }
        } else {
            printf("[LOI] Lua chon khong hop le! Vui long chon tu 0 den 5.\n");
        }
    }
    return 0;
}
