/*
 * run_tests.c - Chay ma tran test: 4 thuat toan x cac tep test.
 * Moi ca: ma hoa -> giai ma -> so sanh tep ket qua voi tep goc.
 *
 * Chay tu BEN TRONG thu muc tests/:
 *   mkdir -p data out
 *   gcc -Wall -Wextra -std=c11 -pedantic gen_data.c -o gen_data
 *   gcc -Wall -Wextra -std=c11 -pedantic run_tests.c -o run_tests
 *   ./gen_data
 *   ./run_tests
 */
#include <stdio.h>
#include <stdlib.h>

#define CMD_LEN 512
#define PATH_LEN 256
#define CIPHER_EXE "../cipher" //sua cho nay cho dung ten
#define DATA_DIR "data"
#define OUT_DIR "out"
#define NUM_ALGOS 4
#define NUM_FILES 12

/* SUA: ten 4 thuat toan va khoa tuong ung cho dung voi main.c cua nhom */
static const char *ALGOS[] = {"caesar", "xor", "algo3", "algo4"};
static const char *KEYS[] = {"3", "secretkey", "key3", "key4"};

/* Ten cac tep do gen_data.c tao ra trong thu muc data/ */
static const char *FILES[] = {
    "0bit.bin", "8bit.bin", "56bit.bin", "64bit.bin", "72bit.bin",
    "2048bit.bin", "8192bit.bin", "524280bit.bin", "524288bit.bin",
    "524296bit.bin", "tiengviet.txt", "838860800bit.bin"
};

/* Chay chuong trinh ma hoa. Tra ve 0 neu thanh cong, khac 0 neu loi */
static int run_cipher(const char *algo, const char *op, const char *key,
                      const char *in, const char *out)
{
    char cmd[CMD_LEN];

    /* SUA dong nay cho dung cu phap that cua main.c */
    snprintf(cmd, sizeof(cmd), "%s %s %s %s \"%s\" \"%s\"",
             CIPHER_EXE, algo, op, key, in, out);
    return system(cmd);
}

/* So sanh 2 tep tung byte. Tra ve 1 neu giong nhau, 0 neu khac */
static int files_equal(const char *path_a, const char *path_b)
{
    FILE *fa = fopen(path_a, "rb");
    FILE *fb = fopen(path_b, "rb");
    int same = 1;
    int ca;
    int cb;

    if (fa == NULL || fb == NULL) {
        same = 0;
    } else {
        do {
            ca = fgetc(fa);
            cb = fgetc(fb);
            if (ca != cb) {
                same = 0;
                break;
            }
        } while (ca != EOF);
    }
    if (fa != NULL) {
        fclose(fa);
    }
    if (fb != NULL) {
        fclose(fb);
    }
    return same;
}

/* Test 1 ca: ma hoa -> giai ma -> so sanh. Tra ve 1 neu PASS */
static int test_one(const char *algo, const char *key, const char *name)
{
    char src[PATH_LEN];
    char enc[PATH_LEN];
    char dec[PATH_LEN];
    int ok = 0;

    snprintf(src, sizeof(src), "%s/%s", DATA_DIR, name);
    snprintf(enc, sizeof(enc), "%s/%s.enc", OUT_DIR, name);
    snprintf(dec, sizeof(dec), "%s/%s.dec", OUT_DIR, name);

    if (run_cipher(algo, "enc", key, src, enc) == 0 &&
        run_cipher(algo, "dec", key, enc, dec) == 0 &&
        files_equal(src, dec)) {
        ok = 1;
    }
    remove(enc);
    remove(dec);
    return ok;
}

int main(void)
{
    int total = 0;
    int failed = 0;
    int i;
    int j;

    printf("%-10s %-20s %s\n", "ALGO", "FILE", "KET QUA");
    for (i = 0; i < NUM_ALGOS; i++) {
        for (j = 0; j < NUM_FILES; j++) {
            int ok = test_one(ALGOS[i], KEYS[i], FILES[j]);

            printf("%-10s %-20s %s\n", ALGOS[i], FILES[j],
                   ok ? "PASS" : "FAIL");
            total++;
            if (!ok) {
                failed++;
            }
        }
    }
    printf("\nTong: %d ca, %d FAIL\n", total, failed);
    return failed > 0 ? 1 : 0;
}
