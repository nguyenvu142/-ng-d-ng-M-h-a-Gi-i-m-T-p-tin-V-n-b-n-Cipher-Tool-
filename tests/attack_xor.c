// Còn sửa lại vì chưa có XOR chính.





#include <stdio.h>

#define MAX_BYTE 4000

/* Doc toi da MAX_BYTE byte tu tep. Tra ve so byte doc duoc, -1 neu loi */
static int doc_tep(const char *ten_tep, unsigned char *du_lieu) {
    FILE *f = fopen(ten_tep, "rb");
    int n;

    if (f == NULL) {
        return -1;
    }
    n = (int)fread(du_lieu, 1, MAX_BYTE, f);
    fclose(f);
    return n;
}

/* Tim chu ky nho nhat cua day khoa (khoa XOR lap lai). 0 neu khong tim thay */
static int tim_chu_ky(const unsigned char *day_khoa, int n) {
    int chu_ky;
    int i;
    int khop;

    for (chu_ky = 1; chu_ky <= n / 2; chu_ky++) {
        khop = 1;
        for (i = 0; i + chu_ky < n; i++) {
            if (day_khoa[i] != day_khoa[i + chu_ky]) {
                khop = 0;
                break;
            }
        }
        if (khop) {
            return chu_ky;
        }
    }
    return 0;
}

int main(void) {
    char tep_biet[512];
    char tep_ma[512];
    unsigned char ro[MAX_BYTE];
    unsigned char ma[MAX_BYTE];
    unsigned char day_khoa[MAX_BYTE];
    int n_ro;
    int n_ma;
    int i;
    int chu_ky;
    FILE *f;

    printf("Tep chua doan ban ro ke tan cong biet: ");
    if (scanf("%511s", tep_biet) != 1) {
        return 1;
    }
    printf("Tep ban ma: ");
    if (scanf("%511s", tep_ma) != 1) {
        return 1;
    }
    n_ro = doc_tep(tep_biet, ro);
    n_ma = doc_tep(tep_ma, ma);
    if (n_ro <= 0 || n_ma < n_ro) {
        printf("Loi doc tep (tep ban ma phai dai it nhat bang doan ban ro)\n");
        return 1;
    }

    /* Y TUONG: ban_ma = ban_ro XOR khoa  =>  khoa = ban_ma XOR ban_ro */
    for (i = 0; i < n_ro; i++) {
        day_khoa[i] = (unsigned char)(ma[i] ^ ro[i]);
    }
    printf("\nDay khoa tim duoc (%d byte): ", n_ro);
    for (i = 0; i < n_ro; i++) {
        printf("%02X ", day_khoa[i]);
    }
    printf("\n");

    chu_ky = tim_chu_ky(day_khoa, n_ro);
    if (chu_ky == 0) {
        printf("Khong thay chu ky: khoa dai hon nua doan ban ro biet.\n");
        return 0;
    }
    printf("Khoa lap lai sau %d byte, khoa la: ", chu_ky);
    for (i = 0; i < chu_ky; i++) {
        putchar((day_khoa[i] >= 32 && day_khoa[i] <= 126) ? day_khoa[i] : '.');
    }
    printf("\n");

    /* Co khoa roi thi giai ma TOAN BO tep */
    f = fopen("out/xor_cracked.txt", "wb");
    if (f == NULL) {
        printf("Khong tao duoc out/xor_cracked.txt\n");
        return 1;
    }
    for (i = 0; i < n_ma; i++) {
        fputc(ma[i] ^ day_khoa[i % chu_ky], f);
    }
    fclose(f);
    printf("Da giai ma toan bo %d byte -> out/xor_cracked.txt\n", n_ma);
    return 0;
}
