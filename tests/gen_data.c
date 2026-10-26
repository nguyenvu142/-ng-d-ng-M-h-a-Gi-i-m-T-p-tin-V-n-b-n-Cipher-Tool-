#include <stdio.h>

int main(void) {
    FILE *f;
    int i;

    /* ===== 0 bit (0 byte): mo ra roi dong, khong ghi gi ===== */
    f = fopen("data/0bit.bin", "wb");
    if (f == NULL) { printf("Loi: chua co thu muc data\n"); return 1; }
    fclose(f);

    /* ===== 8 bit (1 byte) ===== */
    f = fopen("data/8bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    fputc('A', f);
    fclose(f);

    /* ===== 56 bit (7 byte): thieu 1 byte so voi khoi hoan vi 8 ===== */
    f = fopen("data/56bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 7; i++) {
        fputc(i + 1, f);
    }
    fclose(f);

    /* ===== 64 bit (8 byte): dung 1 khoi hoan vi ===== */
    f = fopen("data/64bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 8; i++) {
        fputc(i + 1, f);
    }
    fclose(f);

    /* ===== 72 bit (9 byte): du 1 byte sang khoi thu hai ===== */
    f = fopen("data/72bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 9; i++) {
        fputc(i + 1, f);
    }
    fclose(f);

    /* ===== 2048 bit (256 byte): du moi gia tri tu 0 den 255 ===== */
    f = fopen("data/2048bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 256; i++) {
        fputc(i, f);
    }
    fclose(f);

    /* ===== 8192 bit (1024 byte = 1 KB) ===== */
    f = fopen("data/8192bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 1024; i++) {
        fputc((i * 7) % 256, f);
    }
    fclose(f);

    /* ===== 524280 bit (65535 byte) = BLOCK_SIZE - 1 ===== */
    f = fopen("data/524280bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 65535; i++) {
        fputc(i % 256, f);
    }
    fclose(f);

    /* ===== 524288 bit (65536 byte) = BLOCK_SIZE ===== */
    f = fopen("data/524288bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 65536; i++) {
        fputc(i % 256, f);
    }
    fclose(f);

    /* ===== 524296 bit (65537 byte) = BLOCK_SIZE + 1 ===== */
    f = fopen("data/524296bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 65537; i++) {
        fputc(i % 256, f);
    }
    fclose(f);

    /* ===== Tieng Viet UTF-8 (101 byte) =====
       Viet bang ma byte de khong phu thuoc cach luu file.
       Dong 1: "Truong Dai hoc Giao thong van tai" (co dau)
       Dong 2: "Ung dung ma hoa tep tin: a a e o o u d" (co dau) */
    f = fopen("data/tiengviet.txt", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    fputs("Tr\306\260\341\273\235ng \304\220\341\272\241i h\341\273\215c "
          "Giao th\303\264ng v\341\272\255n t\341\272\243i\n", f);
    fputs("\341\273\250ng d\341\273\245ng m\303\243 h\303\263a t\341\273\207p tin: "
          "\304\203 \303\242 \303\252 \303\264 \306\241 \306\260 \304\221\n", f);
    fclose(f);

    /* ===== 838860800 bit (104857600 byte = 100 MB) ===== */
    f = fopen("data/838860800bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 104857600; i++) {
        fputc(i % 256, f);
    }
    fclose(f);

    printf("Xong\n");
    return 0;
}
