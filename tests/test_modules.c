#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "test_common.h"
#include "padding.h"
#include "permutation_module.h"
#include "transpose_process.h"

/* Test muc HAM cho cac module cua Tuan (XOR, hoan vi, padding), vi menu cua Huy
   chua co cac thuat toan nay. Moi tep nguon cua Tuan co main() rieng, nen khi dich
   phai doi ten: -Dmain=xor_demo_main (xem lenh dich o cuoi tep). */

void xor_encrypt(const uint8_t *input, uint8_t *output, size_t length,
                 const uint8_t *key, size_t key_len);
void xor_decrypt(const uint8_t *input, uint8_t *output, size_t length,
                 const uint8_t *key, size_t key_len);

#ifdef _WIN32
#define THIET_BI_NULL "nul"
#else
#define THIET_BI_NULL "/dev/null"
#endif

#define LOI_KHONG_DUNG (-99)
#define SO_TEP 12
#define TEP_KET_QUA "ketqua_modules.txt"

typedef struct {
    FILE *ket_qua;
    int so_pass;
    int so_fail;
} ThongKe;

static void ghi_ket_qua(ThongKe *tk, const char *nhom, const char *ten,
                        int ma_loi) {
    const char *nhan = (ma_loi == SUCCESS) ? "PASS" : "FAIL";

    printf("%s | %s | %s | ma=%d\n", nhan, nhom, ten, ma_loi);
    fprintf(tk->ket_qua, "%s | %s | %s | ma=%d\n", nhan, nhom, ten, ma_loi);
    if (ma_loi == SUCCESS) {
        tk->so_pass++;
    } else {
        tk->so_fail++;
    }
}

/* Tao day byte co dinh, co ca byte 0 va 255 */
static void tao_du_lieu(uint8_t *buf, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        buf[i] = (uint8_t)((i * 7u + 3u) % 256u);
    }
}

/* ---------------- XOR ---------------- */

/* Ma hoa roi giai ma, ket qua phai giong du lieu goc */
static int thu_xor_vong_tron(size_t n, size_t do_dai_khoa) {
    uint8_t *goc = malloc(n + 1);
    uint8_t *ma = malloc(n + 1);
    uint8_t *giai = malloc(n + 1);
    uint8_t khoa[256];
    int ma_loi = SUCCESS;
    size_t i;

    if (goc == NULL || ma == NULL || giai == NULL) {
        free(goc);
        free(ma);
        free(giai);
        return ERR_MEM_ALLOC;
    }
    tao_du_lieu(goc, n);
    for (i = 0; i < do_dai_khoa; i++) {
        khoa[i] = (uint8_t)(0x55u + i * 13u);
    }
    xor_encrypt(goc, ma, n, khoa, do_dai_khoa);
    xor_decrypt(ma, giai, n, khoa, do_dai_khoa);
    if (n > 0 && memcmp(goc, giai, n) != 0) {
        ma_loi = ERR_CRC_MISMATCH;
    }
    free(goc);
    free(ma);
    free(giai);
    return ma_loi;
}

/* Ban ma phai KHAC ban ro (neu khong thi ma hoa vo nghia) */
static int thu_xor_co_thay_doi(void) {
    uint8_t goc[64];
    uint8_t ma[64];
    const uint8_t khoa[] = "KhoaBiMat";

    tao_du_lieu(goc, sizeof(goc));
    xor_encrypt(goc, ma, sizeof(goc), khoa, sizeof(khoa) - 1);
    if (memcmp(goc, ma, sizeof(goc)) == 0) {
        return LOI_KHONG_DUNG;
    }
    return SUCCESS;
}

/* Giai ma bang SAI khoa: ket qua KHONG duoc giong ban goc */
static int thu_xor_sai_khoa(void) {
    uint8_t goc[64];
    uint8_t ma[64];
    uint8_t giai[64];
    const uint8_t khoa_dung[] = "KhoaDung";
    const uint8_t khoa_sai[] = "KhoaSaiX";

    tao_du_lieu(goc, sizeof(goc));
    xor_encrypt(goc, ma, sizeof(goc), khoa_dung, sizeof(khoa_dung) - 1);
    xor_decrypt(ma, giai, sizeof(giai), khoa_sai, sizeof(khoa_sai) - 1);
    if (memcmp(goc, giai, sizeof(goc)) == 0) {
        return LOI_KHONG_DUNG;
    }
    return SUCCESS;
}

/* Cac ca "nguy hiem": chay trong tien trinh con, vi co the lam chuong trinh sap.
   Chuong trinh phai TU CHOI dau vao xau, khong duoc sap. */
static int chay_ca_nguy_hiem(const char *ten_ca) {
    uint8_t vao[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t ra[8];
    uint8_t khoa[1] = {0x55};

    if (strcmp(ten_ca, "xor_khoa_rong") == 0) {
        xor_encrypt(vao, ra, sizeof(vao), khoa, 0);   /* do dai khoa = 0 */
    } else if (strcmp(ten_ca, "xor_khoa_null") == 0) {
        xor_encrypt(vao, ra, sizeof(vao), NULL, 1);   /* con tro khoa = NULL */
    } else if (strcmp(ten_ca, "xor_vao_null") == 0) {
        xor_encrypt(NULL, ra, sizeof(vao), khoa, 1);  /* du lieu vao = NULL */
    }
    return 0;
}

static int thu_ca_nguy_hiem(const char *exe, const char *ten_ca) {
    char lenh[600];

    snprintf(lenh, sizeof(lenh), "\"%s\" %s > %s 2>&1", exe, ten_ca, THIET_BI_NULL);
    if (system(lenh) != 0) {
        return LOI_KHONG_DUNG;     /* chuong trinh bi sap thay vi tu choi */
    }
    return SUCCESS;
}

/* ---------------- Hoan vi ---------------- */

/* Moi gia tri 0..7 phai xuat hien dung 1 lan */
static int la_hoan_vi_hop_le(const uint8_t *perm, size_t n) {
    int da_thay[PERM_MODULE_BLOCK_SIZE] = {0};
    size_t i;

    for (i = 0; i < n; i++) {
        if (perm[i] >= n || da_thay[perm[i]]) {
            return 0;
        }
        da_thay[perm[i]] = 1;
    }
    return 1;
}

/* Voi nhieu khoa khac nhau: perm hop le, va hoan vi roi nghich dao thi ra ban dau */
static int thu_hoan_vi_nhieu_khoa(void) {
    uint8_t khoa[16];
    uint8_t perm[PERM_MODULE_BLOCK_SIZE];
    uint8_t nghich[PERM_MODULE_BLOCK_SIZE];
    uint8_t goc[PERM_MODULE_BLOCK_SIZE] = {10, 20, 30, 40, 50, 60, 70, 80};
    uint8_t xao[PERM_MODULE_BLOCK_SIZE];
    uint8_t lai[PERM_MODULE_BLOCK_SIZE];
    size_t do_dai;
    size_t i;
    int lan;

    for (lan = 0; lan < 300; lan++) {
        do_dai = (size_t)(lan % 16) + 1;
        for (i = 0; i < do_dai; i++) {
            khoa[i] = (uint8_t)(lan * 31 + (int)i * 17);
        }
        perm_from_key(khoa, do_dai, perm);
        if (!la_hoan_vi_hop_le(perm, PERM_MODULE_BLOCK_SIZE)) {
            return ERR_INVALID_KEY;
        }
        get_inverse_permutation(perm, nghich, PERM_MODULE_BLOCK_SIZE);
        apply_permutation(goc, xao, perm, PERM_MODULE_BLOCK_SIZE);
        apply_permutation(xao, lai, nghich, PERM_MODULE_BLOCK_SIZE);
        if (memcmp(goc, lai, sizeof(goc)) != 0) {
            return ERR_CRC_MISMATCH;
        }
    }
    return SUCCESS;
}

/* transpose_block / inverse_transpose_block ket hop voi perm_from_key */
static int thu_transpose_vong_tron(void) {
    const uint8_t khoa[] = "UTC2";
    uint8_t perm[TRANSPOSE_BLOCK_SIZE];
    uint8_t nghich[TRANSPOSE_BLOCK_SIZE];
    uint8_t goc[TRANSPOSE_BLOCK_SIZE] = {'U', 'T', 'C', '2', 'C', 'O', 'D', 'E'};
    uint8_t xao[TRANSPOSE_BLOCK_SIZE];
    uint8_t lai[TRANSPOSE_BLOCK_SIZE];

    perm_from_key(khoa, sizeof(khoa) - 1, perm);
    get_inverse_permutation(perm, nghich, TRANSPOSE_BLOCK_SIZE);
    transpose_block(goc, xao, perm);
    inverse_transpose_block(xao, lai, nghich);
    if (memcmp(goc, lai, sizeof(goc)) != 0) {
        return ERR_CRC_MISMATCH;
    }
    return SUCCESS;
}

/* Khoa rong: perm_from_key khong duoc de mang perm chua khoi tao
   (neu de nguyen thi apply_permutation se doc ngoai mang) */
static int thu_hoan_vi_khoa_rong(void) {
    uint8_t perm[PERM_MODULE_BLOCK_SIZE];
    const uint8_t khoa[1] = {0};
    size_t i;

    memset(perm, 0xAA, sizeof(perm));
    perm_from_key(khoa, 0, perm);
    for (i = 0; i < sizeof(perm); i++) {
        if (perm[i] != 0xAA) {
            return SUCCESS;           /* ham da ghi gi do vao perm */
        }
    }
    return LOI_KHONG_DUNG;            /* perm van la rac: ham im lang bo qua */
}

/* ---------------- Padding PKCS7 ---------------- */

static int thu_padding_do_dai(size_t n) {
    uint8_t vao[64];
    uint8_t ra[80];
    size_t do_dai_ra = 0;
    size_t do_dai_goc = 0;
    size_t so_dem;
    size_t i;

    tao_du_lieu(vao, n);
    pkcs7_pad(vao, n, ra, &do_dai_ra);
    if (do_dai_ra % PADDING_BLOCK_SIZE != 0 || do_dai_ra <= n) {
        return ERR_INVALID_PADDING;
    }
    so_dem = do_dai_ra - n;
    for (i = n; i < do_dai_ra; i++) {
        if (ra[i] != (uint8_t)so_dem) {
            return ERR_INVALID_PADDING;
        }
    }
    if (pkcs7_unpad(ra, do_dai_ra, &do_dai_goc) != 1 || do_dai_goc != n
        || memcmp(ra, vao, n) != 0) {
        return ERR_INVALID_PADDING;
    }
    return SUCCESS;
}

/* Padding sai dinh dang phai bi tu choi (unpad tra ve 0) */
static int thu_padding_xau(const char *kieu) {
    uint8_t buf[16];
    size_t dai = 16;
    size_t ket_qua = 0;
    int tra_ve;

    memset(buf, 7, sizeof(buf));
    if (strcmp(kieu, "byte cuoi = 0") == 0) {
        buf[15] = 0;
    } else if (strcmp(kieu, "byte cuoi = 9 (lon hon khoi)") == 0) {
        buf[15] = 9;
    } else if (strcmp(kieu, "cac byte dem khong dong nhat") == 0) {
        buf[15] = 4;
        buf[14] = 4;
        buf[13] = 4;
        buf[12] = 3;
    } else if (strcmp(kieu, "do dai khong chia het 8") == 0) {
        dai = 15;
    }
    tra_ve = pkcs7_unpad(buf, dai, &ket_qua);
    return (tra_ve == 0) ? SUCCESS : LOI_KHONG_DUNG;
}

/* Truong hop tep rong: khong co du lieu (con tro NULL) nhung van phai co 8 byte dem */
static int thu_padding_tep_rong_null(void) {
    uint8_t ra[16];
    size_t do_dai_ra = 12345;

    pkcs7_pad(NULL, 0, ra, &do_dai_ra);
    return (do_dai_ra == PADDING_BLOCK_SIZE) ? SUCCESS : LOI_KHONG_DUNG;
}

/* ---------------- Ghep: pad -> hoan vi tung khoi -> XOR (theo ghi chu cua Tuan) ---------------- */

static int doc_ca_tep(const char *ten, uint8_t **du_lieu, size_t *n) {
    FILE *f = fopen(ten, "rb");
    long kich_thuoc;

    if (f == NULL) {
        return ERR_FILE_NOT_FOUND;
    }
    fseek(f, 0, SEEK_END);
    kich_thuoc = ftell(f);
    fseek(f, 0, SEEK_SET);
    *n = (size_t)kich_thuoc;
    *du_lieu = malloc(*n + PADDING_BLOCK_SIZE + 1);
    if (*du_lieu == NULL) {
        fclose(f);
        return ERR_MEM_ALLOC;
    }
    if (fread(*du_lieu, 1, *n, f) != *n) {
        fclose(f);
        free(*du_lieu);
        return ERR_FILE_READ;
    }
    fclose(f);
    return SUCCESS;
}

/* Ma hoa: pad, hoan vi tung khoi 8 byte, roi XOR. Giai ma theo chieu nguoc lai. */
static int thu_ghep_mot_tep(const char *ten_tep) {
    const uint8_t khoa[] = "KhoaGhep";
    size_t khoa_dai = sizeof(khoa) - 1;
    uint8_t *goc = NULL;
    uint8_t *dem = NULL;
    uint8_t *ma = NULL;
    uint8_t perm[PERM_MODULE_BLOCK_SIZE];
    uint8_t nghich[PERM_MODULE_BLOCK_SIZE];
    uint8_t khoi[PERM_MODULE_BLOCK_SIZE];
    size_t n = 0;
    size_t n_dem = 0;
    size_t n_goc = 0;
    size_t i;
    int ma_loi = doc_ca_tep(ten_tep, &goc, &n);

    if (ma_loi != SUCCESS) {
        return ma_loi;
    }
    dem = malloc(n + PADDING_BLOCK_SIZE);
    ma = malloc(n + PADDING_BLOCK_SIZE);
    if (dem == NULL || ma == NULL) {
        free(goc);
        free(dem);
        free(ma);
        return ERR_MEM_ALLOC;
    }
    perm_from_key(khoa, khoa_dai, perm);
    get_inverse_permutation(perm, nghich, PERM_MODULE_BLOCK_SIZE);

    pkcs7_pad(n > 0 ? goc : (const uint8_t *)"", n, dem, &n_dem);
    for (i = 0; i < n_dem; i += PERM_MODULE_BLOCK_SIZE) {
        apply_permutation(dem + i, khoi, perm, PERM_MODULE_BLOCK_SIZE);
        memcpy(dem + i, khoi, PERM_MODULE_BLOCK_SIZE);
    }
    xor_encrypt(dem, ma, n_dem, khoa, khoa_dai);

    xor_decrypt(ma, dem, n_dem, khoa, khoa_dai);
    for (i = 0; i < n_dem; i += PERM_MODULE_BLOCK_SIZE) {
        apply_permutation(dem + i, khoi, nghich, PERM_MODULE_BLOCK_SIZE);
        memcpy(dem + i, khoi, PERM_MODULE_BLOCK_SIZE);
    }
    if (pkcs7_unpad(dem, n_dem, &n_goc) != 1) {
        ma_loi = ERR_INVALID_PADDING;
    } else if (n_goc != n || (n > 0 && memcmp(dem, goc, n) != 0)) {
        ma_loi = ERR_CRC_MISMATCH;
    }
    free(goc);
    free(dem);
    free(ma);
    return ma_loi;
}

/* ---------------- Dap an da biet cua cac module (TINH TAY, khong lay tu ham dang test) ---------------- */

/* So khop hai day byte: SUCCESS neu giong, ERR_CRC_MISMATCH neu khac */
static int so_khop(const uint8_t *a, const uint8_t *b, size_t n) {
    return (memcmp(a, b, n) == 0) ? SUCCESS : ERR_CRC_MISMATCH;
}

/* XOR roi xoay trai 3 bit. Vi du 0x41 ^ 0x55 = 0x14 = 00010100 -> 10100000 = 0xA0 */
static int thu_dap_an_xor(void) {
    const uint8_t vao1[1] = {0x41};
    const uint8_t khoa1[1] = {0x55};
    const uint8_t mong1[1] = {0xA0};
    const uint8_t vao2[3] = {0x01, 0x02, 0x03};
    const uint8_t khoa2[2] = {0xAA, 0x0F};      /* khoa lap lai: AA 0F AA */
    const uint8_t mong2[3] = {0x5D, 0x68, 0x4D};
    uint8_t ra[3];

    xor_encrypt(vao1, ra, 1, khoa1, 1);
    if (so_khop(ra, mong1, 1) != SUCCESS) {
        return ERR_CRC_MISMATCH;
    }
    xor_encrypt(vao2, ra, 3, khoa2, 2);
    return so_khop(ra, mong2, 3);
}

static int thu_dap_an_xor_giai_ma(void) {
    const uint8_t ma[1] = {0xA0};
    const uint8_t khoa[1] = {0x55};
    const uint8_t mong[1] = {0x41};
    uint8_t ra[1];

    xor_decrypt(ma, ra, 1, khoa, 1);
    return so_khop(ra, mong, 1);
}

/* apply_permutation: ra[i] = vao[perm[i]] */
static int thu_dap_an_hoan_vi(const uint8_t *perm, const char *mong) {
    const uint8_t vao[PERM_MODULE_BLOCK_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    uint8_t ra[PERM_MODULE_BLOCK_SIZE];

    apply_permutation(vao, ra, perm, PERM_MODULE_BLOCK_SIZE);
    return so_khop(ra, (const uint8_t *)mong, PERM_MODULE_BLOCK_SIZE);
}

/* pkcs7_pad: them (8 - n % 8) byte, moi byte co gia tri bang so byte them */
static int thu_dap_an_padding(const uint8_t *vao, size_t n, const uint8_t *mong,
                              size_t n_mong) {
    uint8_t ra[32];
    size_t do_dai_ra = 0;

    pkcs7_pad(vao, n, ra, &do_dai_ra);
    if (do_dai_ra != n_mong) {
        return ERR_INVALID_PADDING;
    }
    return so_khop(ra, mong, n_mong);
}

static void chay_dap_an_module(ThongKe *tk) {
    const uint8_t perm_doi_cho[8] = {1, 0, 3, 2, 5, 4, 7, 6};
    const uint8_t perm_dich[8] = {1, 2, 3, 4, 5, 6, 7, 0};
    const uint8_t perm_dao[8] = {7, 6, 5, 4, 3, 2, 1, 0};
    const uint8_t abc[3] = {'A', 'B', 'C'};
    const uint8_t pad_abc[8] = {'A', 'B', 'C', 5, 5, 5, 5, 5};
    const uint8_t tam_byte[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    const uint8_t pad_tam[16] = {1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8};
    const uint8_t rong[1] = {0};
    const uint8_t pad_rong[8] = {8, 8, 8, 8, 8, 8, 8, 8};

    ghi_ket_qua(tk, "dap-an-module", "XOR: 0x41 khoa 0x55 -> 0xA0; khoa lap lai",
                thu_dap_an_xor());
    ghi_ket_qua(tk, "dap-an-module", "XOR giai ma: 0xA0 khoa 0x55 -> 0x41",
                thu_dap_an_xor_giai_ma());
    ghi_ket_qua(tk, "dap-an-module", "hoan vi doi cho cap: ABCDEFGH -> BADCFEHG",
                thu_dap_an_hoan_vi(perm_doi_cho, "BADCFEHG"));
    ghi_ket_qua(tk, "dap-an-module", "hoan vi dich vong: ABCDEFGH -> BCDEFGHA",
                thu_dap_an_hoan_vi(perm_dich, "BCDEFGHA"));
    ghi_ket_qua(tk, "dap-an-module", "hoan vi dao nguoc: ABCDEFGH -> HGFEDCBA",
                thu_dap_an_hoan_vi(perm_dao, "HGFEDCBA"));
    ghi_ket_qua(tk, "dap-an-module", "padding: ABC -> ABC + 5 byte 0x05",
                thu_dap_an_padding(abc, 3, pad_abc, 8));
    ghi_ket_qua(tk, "dap-an-module", "padding: 8 byte -> them nguyen 1 khoi 0x08",
                thu_dap_an_padding(tam_byte, 8, pad_tam, 16));
    ghi_ket_qua(tk, "dap-an-module", "padding: 0 byte -> 8 byte 0x08",
                thu_dap_an_padding(rong, 0, pad_rong, 8));
}

/* ---------------- Chay het ---------------- */

static void chay_xor(ThongKe *tk, const char *exe) {
    const size_t do_dai[] = {0, 1, 7, 8, 9, 255, 256, 257, 65535, 65536, 65537};
    const size_t khoa_dai[] = {1, 3, 8, 15, 256};
    char ten[100];
    size_t i;
    size_t k;

    for (i = 0; i < sizeof(do_dai) / sizeof(do_dai[0]); i++) {
        for (k = 0; k < sizeof(khoa_dai) / sizeof(khoa_dai[0]); k++) {
            snprintf(ten, sizeof(ten), "vong tron %lu byte, khoa %lu byte",
                     (unsigned long)do_dai[i], (unsigned long)khoa_dai[k]);
            ghi_ket_qua(tk, "xor", ten, thu_xor_vong_tron(do_dai[i], khoa_dai[k]));
        }
    }
    ghi_ket_qua(tk, "xor", "ban ma khac ban ro", thu_xor_co_thay_doi());
    ghi_ket_qua(tk, "xor", "giai ma sai khoa", thu_xor_sai_khoa());
    ghi_ket_qua(tk, "xor-am-tinh", "do dai khoa = 0 (khong duoc sap)",
                thu_ca_nguy_hiem(exe, "xor_khoa_rong"));
    ghi_ket_qua(tk, "xor-am-tinh", "con tro khoa = NULL (khong duoc sap)",
                thu_ca_nguy_hiem(exe, "xor_khoa_null"));
    ghi_ket_qua(tk, "xor-am-tinh", "du lieu vao = NULL (khong duoc sap)",
                thu_ca_nguy_hiem(exe, "xor_vao_null"));
}

static void chay_hoan_vi_va_padding(ThongKe *tk) {
    const char *kieu_xau[4] = {"byte cuoi = 0", "byte cuoi = 9 (lon hon khoi)",
                               "cac byte dem khong dong nhat",
                               "do dai khong chia het 8"};
    char ten[100];
    size_t n;
    int i;

    ghi_ket_qua(tk, "hoan-vi", "300 khoa: hop le + nghich dao",
                thu_hoan_vi_nhieu_khoa());
    ghi_ket_qua(tk, "hoan-vi", "transpose_block khoi 8 byte",
                thu_transpose_vong_tron());
    ghi_ket_qua(tk, "hoan-vi-am-tinh", "khoa rong: perm khong duoc de rac",
                thu_hoan_vi_khoa_rong());
    for (n = 0; n <= 40; n++) {
        snprintf(ten, sizeof(ten), "pad/unpad %lu byte", (unsigned long)n);
        ghi_ket_qua(tk, "padding", ten, thu_padding_do_dai(n));
    }
    for (i = 0; i < 4; i++) {
        snprintf(ten, sizeof(ten), "tu choi: %s", kieu_xau[i]);
        ghi_ket_qua(tk, "padding-am-tinh", ten, thu_padding_xau(kieu_xau[i]));
    }
    ghi_ket_qua(tk, "padding-am-tinh", "tep rong (NULL, 0) van phai dem 8 byte",
                thu_padding_tep_rong_null());
}

static void chay_ghep(ThongKe *tk) {
    const char *tep[SO_TEP] = {
        "data/0bit.bin",       "data/8bit.bin",       "data/56bit.bin",
        "data/64bit.bin",      "data/72bit.bin",      "data/2048bit.bin",
        "data/8192bit.bin",    "data/524280bit.bin",  "data/524288bit.bin",
        "data/524296bit.bin",  "data/tiengviet.txt",  "data/838860800bit.bin"
    };
    int j;

    for (j = 0; j < SO_TEP; j++) {
        ghi_ket_qua(tk, "ghep-pad-hoanvi-xor", tep[j], thu_ghep_mot_tep(tep[j]));
    }
}

int main(int argc, char *argv[]) {
    ThongKe tk;

    if (argc == 2) {
        return chay_ca_nguy_hiem(argv[1]);     /* che do tien trinh con */
    }
    tk.so_pass = 0;
    tk.so_fail = 0;
    tk.ket_qua = fopen(TEP_KET_QUA, "w");
    if (tk.ket_qua == NULL) {
        printf("Khong tao duoc %s\n", TEP_KET_QUA);
        return ERR_FILE_OPEN;
    }
    chay_xor(&tk, argv[0]);
    chay_hoan_vi_va_padding(&tk);
    chay_dap_an_module(&tk);
    chay_ghep(&tk);
    printf("=== PASS=%d  FAIL=%d ===\n", tk.so_pass, tk.so_fail);
    fprintf(tk.ket_qua, "=== PASS=%d  FAIL=%d ===\n", tk.so_pass, tk.so_fail);
    fclose(tk.ket_qua);
    return SUCCESS;
}

/* CACH DICH (Windows, tu thu muc goc repo). Moi tep cua Tuan co main() rieng nen doi ten:
   gcc -c -Dmain=xor_demo_main -Itests docs/xorcipher.c -o xor.o
   gcc -c -Dmain=padding_demo_main -Itests tests/padding.c -o padding.o
   gcc -c -Dmain=transpose_demo_main -Itests tests/transpose_process.c -o transpose.o
   gcc -c -Idocs -Itests tests/permutation_module.c -o perm.o
   gcc -Wall -Wextra -std=c11 -pedantic -Idocs -Itests tests/test_modules.c xor.o padding.o transpose.o perm.o -o test_modules */
