#include <stdio.h>
#include "test_common.h"

/* Mot dong trong bang: ten tep, so byte, buoc tang cua gia tri byte */
typedef struct {
    const char *ten_tep;
    long so_byte;
    int buoc;
} MauTep;

/* Ghi so_byte byte, byte thu i co gia tri (i * buoc) % 256 */
static int ghi_tep_so(const char *ten_tep, long so_byte, int buoc) {
    FILE *f = fopen(ten_tep, "wb");
    long i;

    if (f == NULL) {
        return ERR_FILE_OPEN;
    }
    for (i = 0; i < so_byte; i++) {
        if (fputc((int)((i * buoc) % 256), f) == EOF) {
            fclose(f);
            return ERR_FILE_WRITE;
        }
    }
    fclose(f);
    return SUCCESS;
}

/* Ghi nguyen mot mang byte vao tep */
static int ghi_tep_du_lieu(const char *ten_tep, const char *du_lieu, size_t n) {
    FILE *f = fopen(ten_tep, "wb");

    if (f == NULL) {
        return ERR_FILE_OPEN;
    }
    if (fwrite(du_lieu, 1, n, f) != n) {
        fclose(f);
        return ERR_FILE_WRITE;
    }
    fclose(f);
    return SUCCESS;
}

/* Tieng Viet UTF-8 (101 byte), viet bang ma byte de khong phu thuoc cach luu file */
static int tao_tep_tieng_viet(void) {
    static const char noi_dung[] =
        "Tr\306\260\341\273\235ng \304\220\341\272\241i h\341\273\215c "
        "Giao th\303\264ng v\341\272\255n t\341\272\243i\n"
        "\341\273\250ng d\341\273\245ng m\303\243 h\303\263a t\341\273\207p tin: "
        "\304\203 \303\242 \303\252 \303\264 \306\241 \306\260 \304\221\n";

    return ghi_tep_du_lieu("data/tiengviet.txt", noi_dung, sizeof(noi_dung) - 1);
}

int main(void) {
    /* Ten tep = so bit (so byte x 8). Doi BLOCK_SIZE / PERM_BLOCK_N thi doi ten cho khop */
    const MauTep danh_sach[] = {
        {"data/0bit.bin",           0,                  1},
        {"data/8bit.bin",           1,                  1},
        {"data/56bit.bin",          PERM_BLOCK_N - 1,   1},
        {"data/64bit.bin",          PERM_BLOCK_N,       1},
        {"data/72bit.bin",          PERM_BLOCK_N + 1,   1},
        {"data/2048bit.bin",        256,                1},
        {"data/8192bit.bin",        1024,               7},
        {"data/524280bit.bin",      BLOCK_SIZE - 1,     1},
        {"data/524288bit.bin",      BLOCK_SIZE,         1},
        {"data/524296bit.bin",      BLOCK_SIZE + 1,     1},
        {"data/838860800bit.bin",   104857600L,         1}
    };
    const int so_tep = (int)(sizeof(danh_sach) / sizeof(danh_sach[0]));
    int i;
    int ma_loi;

    for (i = 0; i < so_tep; i++) {
        ma_loi = ghi_tep_so(danh_sach[i].ten_tep, danh_sach[i].so_byte,
                            danh_sach[i].buoc);
        if (ma_loi != SUCCESS) {
            printf("Loi %d khi tao %s (da tao thu muc data chua?)\n",
                   ma_loi, danh_sach[i].ten_tep);
            return ma_loi;
        }
    }

    ma_loi = tao_tep_tieng_viet();
    if (ma_loi != SUCCESS) {
        printf("Loi %d khi tao tiengviet.txt\n", ma_loi);
        return ma_loi;
    }

    printf("Xong\n");
    return SUCCESS;
}
