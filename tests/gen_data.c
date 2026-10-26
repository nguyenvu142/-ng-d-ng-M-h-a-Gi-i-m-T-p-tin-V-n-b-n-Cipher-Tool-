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

    /* ===== 32760 bit (4095 byte) = N-1 ===== */
    f = fopen("data/32760bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 4095; i++) {
        fputc(i % 256, f);
    }
    fclose(f);

    /* ===== 32768 bit (4096 byte) = N ===== */
    f = fopen("data/32768bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 4096; i++) {
        fputc(i % 256, f);
    }
    fclose(f);

    /* ===== 32776 bit (4097 byte) = N+1 ===== */
    f = fopen("data/32776bit.bin", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    for (i = 0; i < 4097; i++) {
        fputc(i % 256, f);
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

    /* ===== Tieng Viet co dau (kich thuoc tuy noi dung) ===== */
    f = fopen("data/tiengviet.txt", "wb");
    if (f == NULL) { printf("Loi\n"); return 1; }
    fputs("Trường Đại học Giao thông vận tải\n", f);
    fputs("Ứng dụng mã hóa tệp tin: ă â ê ô ơ ư đ\n", f);
    fclose(f);

    printf("Xong\n");
    return 0;
}
