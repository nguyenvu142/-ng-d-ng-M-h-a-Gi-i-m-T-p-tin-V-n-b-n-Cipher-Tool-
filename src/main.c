#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../cipher.h"
#include "common.h"
#include "fileinfo.h"

/* =========================================================================
 * 1. ĐỊNH NGHĨA CON TRỎ HÀM CHUẨN & CÁC HÀM BỌC (ADAPTER) CHO BẢNG CON TRỎ HÀM
 * Giúp đồng nhất chữ ký hàm của Huy (Caesar, Vigenere) và Tuấn (XOR)
 * ========================================================================= */
typedef void (*ThuatToanFunc)(uint8_t *data, size_t len, const char *raw_key, int mode);

static void boc_caesar(uint8_t *data, size_t len, const char *raw_key, int mode) {
    int shift = atoi(raw_key); /* Đổi chuỗi nhập vào thành số nguyên cho Caesar */
    caesar_process(data, len, shift, mode);
}

static void boc_vigenere(uint8_t *data, size_t len, const char *raw_key, int mode) {
    vigenere_process(data, len, raw_key, mode);
}

static void boc_xor(uint8_t *data, size_t len, const char *raw_key, int mode) {
    (void)mode; /* XOR mã hóa và giải mã giống nhau */
    xor_process(data, len, (const uint8_t *)raw_key, strlen(raw_key));
}

/* =========================================================================
 * 2. BẢNG CON TRỎ HÀM (DISPATCH TABLE) - YÊU CẦU MỐC 3 (VIỆC 3.09)
 * ========================================================================= */
typedef struct {
    int id;
    const char *ten_hien_thi;
    ThuatToanFunc ham_xu_ly;
} MucThuatToan;

static const MucThuatToan BANG_CON_TRO_HAM[] = {
    {1, "Thuat toan Caesar (Dich vong)", boc_caesar},
    {2, "Thuat toan Vigenere (Bang tra)", boc_vigenere},
    {3, "Thuat toan XOR (Bitwise)",      boc_xor}
};

#define SO_THUAT_TOAN (sizeof(BANG_CON_TRO_HAM) / sizeof(BANG_CON_TRO_HAM[0]))
#define BLOCK_SIZE 65536 /* Đọc/ghi theo khối 64 KB */

/* Hàm đọc/ghi tệp nhị phân theo block 64KB và gọi con trỏ hàm từ bảng */
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
        fn(buf, n_read, key, mode); /* Gọi thuật toán thông qua con trỏ hàm */
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

static void xoa_xuong_dong(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[--n] = '\0';
    }
}

/* =========================================================================
 * 3. MENU CHÍNH (TÍCH HỢP MỐC 3 - 3.09 & MỐC 4 - 4.07)
 * ========================================================================= */
int main(void) {
    char line[256], in_file[256], out_file[256], key[256];
    int lua_chon = 0, mode = 1;

    while (1) {
        printf("\n================ CIPHER TOOL - MENU CHINH ================\n");
        for (size_t i = 0; i < SO_THUAT_TOAN; i++) {
            printf("  %d. %s\n", BANG_CON_TRO_HAM[i].id, BANG_CON_TRO_HAM[i].ten_hien_thi);
        }
        printf("  4. Thuat toan Hoan vi khoi (Transpose - Moc 4)\n");
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

            /* Tra cứu con trỏ hàm từ bảng và thực thi */
            ThuatToanFunc fn = BANG_CON_TRO_HAM[lua_chon - 1].ham_xu_ly;
            if (chay_luong_tep(in_file, out_file, key, mode, fn) == 0) {
                printf("[THANH CONG] Da xu ly xong tep -> %s\n", out_file);
            }
        } else if (lua_chon == 4) {
            printf("[Thong bao] Dang goi module Hoan vi khoi (Moc 4)...\n");
        } else if (lua_chon == 5) {
            printf("Nhap duong dan tep can xem thong tin: ");
            if (fgets(in_file, sizeof(in_file), stdin)) {
                xoa_xuong_dong(in_file);
                printf("[Thong bao] Dang doc ThongTinTep cho tep: %s\n", in_file);
            }
        } else {
            printf("[LOI] Lua chon khong hop le! Vui long chon tu 0 den 5.\n");
        }
    }
    return 0;
}
