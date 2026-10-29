#include <stdio.h>
#include <string.h>

#define MAX_BYTE 2000
#define CHE_DO_BYTE 1
#define CHE_DO_CHU  2

/* Giai ma thu mot lan voi khoa k, ket qua nam trong ra[] */
static void giai_ma_thu(const unsigned char *ma, int n, int k, int che_do,
                        unsigned char *ra) {
    int i;
    int c;

    for (i = 0; i < n; i++) {
        c = ma[i];
        if (che_do == CHE_DO_BYTE) {
            c = (c - k + 256) % 256;                       /* dich nguoc k byte */
        } else if (c >= 'A' && c <= 'Z') {
            c = 'A' + (c - 'A' - k + 26) % 26;
        } else if (c >= 'a' && c <= 'z') {
            c = 'a' + (c - 'a' - k + 26) % 26;
        }
        ra[i] = (unsigned char)c;
    }
}

/* Cham diem: ban ro tieng Anh co nhieu ky tu in duoc va nhieu chu hay gap */
static int cham_diem(const unsigned char *ra, int n) {
    int i;
    int diem = 0;

    for (i = 0; i < n; i++) {
        if (ra[i] == '\n' || (ra[i] >= 32 && ra[i] <= 126)) {
            diem++;                                         /* in duoc */
        } else {
            diem -= 5;                                      /* ky tu la: tru nang */
        }
        if (strchr("etaoinETAOIN ", ra[i]) != NULL && ra[i] != 0) {
            diem++;                                         /* chu hay gap */
        }
    }
    return diem;
}

int main(void) {
    char ten_tep[512];
    unsigned char ma[MAX_BYTE];
    unsigned char ra[MAX_BYTE];
    int che_do;
    int n;
    int k;
    int i;
    int diem;
    int diem_max = -1000000;
    int khoa_max = 0;
    int so_khoa;
    FILE *f;

    printf("Tep da ma hoa: ");
    if (scanf("%511s", ten_tep) != 1) {
        return 1;
    }
    printf("Che do cua Caesar (1=byte, 2=chu cai): ");
    if (scanf("%d", &che_do) != 1 || (che_do != 1 && che_do != 2)) {
        return 1;
    }
    f = fopen(ten_tep, "rb");
    if (f == NULL) {
        printf("Khong mo duoc tep\n");
        return 1;
    }
    n = (int)fread(ma, 1, MAX_BYTE, f);
    fclose(f);

    so_khoa = (che_do == CHE_DO_BYTE) ? 255 : 25;
    printf("Thu %d khoa tren %d byte dau...\n\n", so_khoa, n);

    for (k = 1; k <= so_khoa; k++) {
        giai_ma_thu(ma, n, k, che_do, ra);
        diem = cham_diem(ra, n);
        if (diem > diem_max) {
            diem_max = diem;
            khoa_max = k;
        }
        if (che_do == CHE_DO_CHU) {                         /* chi in het cho 25 khoa */
            printf("k=%3d | diem=%5d | ", k, diem);
            for (i = 0; i < n && i < 50; i++) {
                putchar((ra[i] >= 32 && ra[i] <= 126) ? ra[i] : '.');
            }
            printf("\n");
        }
    }

    printf("\n>>> Khoa co diem cao nhat: %d (diem %d)\n", khoa_max, diem_max);
    giai_ma_thu(ma, n, khoa_max, che_do, ra);
    printf(">>> Ban ro tim duoc: ");
    for (i = 0; i < n && i < 100; i++) {
        putchar((ra[i] >= 32 && ra[i] <= 126) ? ra[i] : '.');
    }
    printf("\n");
    return 0;
}
