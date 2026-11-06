/* Kiem thu phan D (Vuong): viec 4.02, 4.03, 4.04.
 * Chay: test_vuong        -> test nho (4.02, 4.03)
 *       test_vuong big    -> them test tep 100 MB (4.04) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "adapter.h"
#include "common.h"
#include "fileio.h"

static int pass_n = 0, fail_n = 0;

static void check(const char *name, int ok)
{
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (ok) {
        pass_n++;
    } else {
        fail_n++;
    }
}

static Key make_key(const char *s)
{
    Key k;
    k.len = strlen(s);
    memcpy(k.data, s, k.len);
    return k;
}

static const char *key_of(int alg)
{
    static const char *k[] = {"", "5", "LEMON", "UTC2key", "hoanvi"};
    return k[alg];
}

static int write_file(const char *path, unsigned long size)
{
    FILE *f = fopen(path, "wb");
    unsigned long i;
    unsigned long x = 2463534242UL;

    if (f == NULL) {
        return 0;
    }
    for (i = 0; i < size; i++) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        fputc((int)(x & 0xFFu), f);
    }
    return fclose(f) == 0;
}

static int same_file(const char *a, const char *b)
{
    FILE *fa = fopen(a, "rb");
    FILE *fb = fopen(b, "rb");
    unsigned char ba[4096], bb[4096];
    int same = (fa != NULL && fb != NULL);

    while (same) {
        size_t na = fread(ba, 1, sizeof ba, fa);
        size_t nb = fread(bb, 1, sizeof bb, fb);
        if (na != nb || memcmp(ba, bb, na) != 0) {
            same = 0;
        } else if (na == 0) {
            break;
        }
    }
    if (fa != NULL) {
        fclose(fa);
    }
    if (fb != NULL) {
        fclose(fb);
    }
    return same;
}

static int exists(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    fclose(f);
    return 1;
}

static ErrCode enc(const char *in, const char *out, int alg, const char *key)
{
    Key k = make_key(key);
    return process_file(in, out, alg, adapter_flags(alg), DEFAULT_BLOCK_N, &k,
                        adapter_lookup(alg));
}

static ErrCode dec(const char *in, const char *out, const char *key)
{
    Key k = make_key(key);
    return decrypt_file(in, out, &k, adapter_lookup);
}

static void cleanup(void)
{
    remove("tv_in.bin"); remove("tv_enc.bin"); remove("tv_dec.bin");
    remove("tv_enc.bin.tmp"); remove("tv_dec.bin.tmp"); remove("tv_bad.bin");
}

static void test_sizes(void)
{
    static const unsigned long sizes[] = {0, 1, 7, 8, 9, 65535, 65536, 65537,
                                          131072, 300001};
    int alg;
    size_t s;
    char name[96];

    printf("\n== 4.02: luong khoi cuoi / padding (4 thuat toan x cac kich thuoc) ==\n");
    for (alg = 1; alg <= 4; alg++) {
        for (s = 0; s < sizeof sizes / sizeof sizes[0]; s++) {
            int ok;
            write_file("tv_in.bin", sizes[s]);
            ok = enc("tv_in.bin", "tv_enc.bin", alg, key_of(alg)) == ERR_OK
              && dec("tv_enc.bin", "tv_dec.bin", key_of(alg)) == ERR_OK
              && same_file("tv_in.bin", "tv_dec.bin");
            snprintf(name, sizeof name, "thuat toan %d, tep %lu byte", alg, sizes[s]);
            check(name, ok);
        }
    }
}

static void test_negative(void)
{
    FILE *f;
    int alg;
    char name[96];

    printf("\n== 4.03: giai ma an toan ==\n");
    write_file("tv_in.bin", 5000);
    for (alg = 1; alg <= 4; alg++) {
        enc("tv_in.bin", "tv_enc.bin", alg, key_of(alg));
        remove("tv_dec.bin");
        snprintf(name, sizeof name, "thuat toan %d: sai khoa -> bao loi, khong de lai tep", alg);
        check(name, dec("tv_enc.bin", "tv_dec.bin", "sai_khoa_9") == ERR_BAD_CRC
                    && !exists("tv_dec.bin") && !exists("tv_dec.bin.tmp"));
        f = fopen("tv_enc.bin", "r+b");
        fseek(f, 40, SEEK_SET);
        fputc(0x5A, f);
        fclose(f);
        remove("tv_dec.bin");
        snprintf(name, sizeof name, "thuat toan %d: sua 1 byte trong tep ma hoa -> bao loi", alg);
        check(name, dec("tv_enc.bin", "tv_dec.bin", key_of(alg)) == ERR_BAD_CRC
                    && !exists("tv_dec.bin"));
    }
    write_file("tv_bad.bin", 100);
    check("tep khong phai do chuong trinh tao -> ERR_BAD_MAGIC",
          dec("tv_bad.bin", "tv_dec.bin", "k") == ERR_BAD_MAGIC);
    check("tep ra trung tep vao (ma hoa) -> ERR_SAME_FILE",
          enc("tv_in.bin", "tv_in.bin", ALG_XOR, "k") == ERR_SAME_FILE);
    check("tep ra trung tep vao (giai ma) -> ERR_SAME_FILE",
          dec("tv_enc.bin", "tv_enc.bin", "k") == ERR_SAME_FILE);
    check("tep khong ton tai -> ERR_NOT_FOUND",
          enc("khong_co_tep.bin", "tv_enc.bin", ALG_XOR, "k") == ERR_NOT_FOUND);
    check("khoa rong -> ERR_EMPTY_KEY",
          enc("tv_in.bin", "tv_enc.bin", ALG_XOR, "") == ERR_EMPTY_KEY);
    check("sau moi loi khong con tep tam .tmp",
          !exists("tv_enc.bin.tmp") && !exists("tv_dec.bin.tmp"));
}

static void test_big(void)
{
    int alg;
    char name[96];

    printf("\n== 4.04: tep 100 MB (4 thuat toan) ==\n");
    printf("Dang tao tep 100 MB...\n");
    write_file("tv_in.bin", 104857600UL);
    for (alg = 1; alg <= 4; alg++) {
        clock_t t0 = clock();
        int ok = enc("tv_in.bin", "tv_enc.bin", alg, key_of(alg)) == ERR_OK;
        clock_t t1 = clock();
        ok = ok && dec("tv_enc.bin", "tv_dec.bin", key_of(alg)) == ERR_OK;
        clock_t t2 = clock();
        ok = ok && same_file("tv_in.bin", "tv_dec.bin");
        snprintf(name, sizeof name,
                 "thuat toan %d: ma hoa %.1f s, giai ma %.1f s, so sanh khop",
                 alg, (double)(t1 - t0) / CLOCKS_PER_SEC,
                 (double)(t2 - t1) / CLOCKS_PER_SEC);
        check(name, ok);
        remove("tv_enc.bin"); remove("tv_dec.bin");
    }
}

int main(int argc, char **argv)
{
    test_sizes();
    test_negative();
    if (argc > 1 && strcmp(argv[1], "big") == 0) {
        test_big();
    }
    cleanup();
    printf("\nTONG KET: %d PASS, %d FAIL\n", pass_n, fail_n);
    return fail_n == 0 ? 0 : 1;
}
