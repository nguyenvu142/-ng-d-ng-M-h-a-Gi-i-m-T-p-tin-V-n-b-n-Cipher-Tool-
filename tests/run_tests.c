#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_common.h"

/* run_tests chay chuong trinh cua nhom nhu mot nguoi dung: ghi san cac cau tra loi cua
   menu vao tep ans.tmp roi chay "cipher < ans.tmp". Repo hien co HAI menu khac nhau:

   1) Menu cua Huy (mac dinh): Caesar va Vigenere, hoi them che do byte/chu cai.
      Dich cipher: xem tests/readme.md hoac chay_test.bat / chay_test.sh.
      Dich run_tests:   gcc -Wall -Wextra -std=c11 -pedantic tests\run_tests.c -o run_tests

   2) Menu cua Vu (src/main.c): Caesar, Vigenere, XOR; tac vu 1=ma hoa, 0=giai ma;
      khong hoi che do byte/chu cai; khoa nhap theo dong nen test duoc khoa rong.
      Dich run_tests:   gcc -Wall -Wextra -std=c11 -pedantic -DMENU_VU tests\run_tests.c -o run_tests_vu

   Windows: CIPHER_CMD mac dinh ".\\cipher.exe". Ubuntu/WSL: dich them -DCIPHER_CMD='"./cipher"'
   Ten chuong trinh khac: -DCIPHER_CMD='".\\cipher_tool.exe"' */
#ifndef CIPHER_CMD
#define CIPHER_CMD ".\\cipher.exe"
#endif

#ifdef MENU_VU
#define SO_THUAT_TOAN 3
#define SO_CHE_DO     1
#else
#define SO_THUAT_TOAN 2
#define SO_CHE_DO     2
#endif

#define TEP_TRA_LOI   "ans.tmp"
#define TEP_NHAT_KY   "log.tmp"
#define TEP_VAO_TAM   "out/in.tmp"
#define TEP_MA_HOA    "out/enc.tmp"
#define TEP_GIAI_MA   "out/dec.tmp"
#define TEP_TRUNG     "out/same.tmp"
#define SO_TEP        12
#define CHE_DO_BYTE   1
#define CHE_DO_CHU    2
/* ma=-99 nghia la: chuong trinh DANG LE phai tu choi nhung lai khong tu choi */
#define LOI_KHONG_TU_CHOI (-99)

typedef struct {
    FILE *ket_qua;
    int so_pass;
    int so_fail;
} ThongKe;

/* In ra man hinh va ghi vao ketqua.txt */
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

/* Chay chuong trinh cua nhom: tra loi lan luot cac cau hoi cua menu, cuoi cung "0" de thoat.
   Menu Huy: thuat toan, che do, tac vu (1/2), tep vao, tep ra, khoa.
   Menu Vu : thuat toan, tac vu (1=ma hoa, 0=giai ma), tep vao, tep ra, khoa.
   Tra ve ma thoat cua chuong trinh (0 = tot). */
static int chay_cipher(AlgoType algo, int che_do, OpMode tac_vu,
                       const char *tep_vao, const char *tep_ra,
                       const char *khoa) {
    char lenh[600];
    FILE *tra_loi = fopen(TEP_TRA_LOI, "w");

    (void)che_do;                      /* menu Vu khong co cau hoi che do */
    if (tra_loi == NULL) {
        return ERR_FILE_OPEN;
    }
#ifdef MENU_VU
    fprintf(tra_loi, "%d\n%d\n%s\n%s\n%s\n0\n", (int)algo,
            (tac_vu == MODE_ENCRYPT) ? 1 : 0, tep_vao, tep_ra, khoa);
#else
    fprintf(tra_loi, "%d\n%d\n%d\n%s\n%s\n%s\n0\n", (int)algo, che_do,
            (int)tac_vu, tep_vao, tep_ra, khoa);
#endif
    fclose(tra_loi);
    snprintf(lenh, sizeof(lenh), "%s < %s > %s", CIPHER_CMD, TEP_TRA_LOI,
             TEP_NHAT_KY);
    return system(lenh);
}

/* Kiem tra truoc khi test: chuong trinh cua nhom co chay duoc khong?
   Chi gui "0" (thoat menu). Neu khong tim thay cipher.exe thi system() tra ve khac 0.
   Tranh truong hop test "PASS gia" vi chuong trinh khong he chay. */
static int kiem_tra_cipher(void) {
    char lenh[600];
    FILE *tra_loi = fopen(TEP_TRA_LOI, "w");

    if (tra_loi == NULL) {
        return ERR_FILE_OPEN;
    }
    fprintf(tra_loi, "0\n");
    fclose(tra_loi);
    snprintf(lenh, sizeof(lenh), "%s < %s > %s", CIPHER_CMD, TEP_TRA_LOI,
             TEP_NHAT_KY);
    if (system(lenh) != 0) {
        return ERR_FILE_NOT_FOUND;
    }
    return SUCCESS;
}

static int tep_ton_tai(const char *ten_tep) {
    FILE *f = fopen(ten_tep, "rb");

    if (f == NULL) {
        return 0;
    }
    fclose(f);
    return 1;
}

/* So sanh hai tep tung byte (nhu lenh cmp).
   Giong het: SUCCESS. Khac noi dung: ERR_CRC_MISMATCH. Khong mo duoc: ERR_FILE_OPEN */
static int so_sanh_tep(const char *tep_a, const char *tep_b) {
    FILE *fa = fopen(tep_a, "rb");
    FILE *fb = fopen(tep_b, "rb");
    int byte_a;
    int byte_b;
    int trang_thai = SUCCESS;

    if (fa == NULL || fb == NULL) {
        trang_thai = ERR_FILE_OPEN;
    } else {
        do {
            byte_a = fgetc(fa);
            byte_b = fgetc(fb);
            if (byte_a != byte_b) {
                trang_thai = ERR_CRC_MISMATCH;
                break;
            }
        } while (byte_a != EOF);
    }
    if (fa != NULL) {
        fclose(fa);
    }
    if (fb != NULL) {
        fclose(fb);
    }
    return trang_thai;
}

/* Chep tep nguon sang tep dich */
static int sao_chep_tep(const char *nguon, const char *dich) {
    FILE *fn = fopen(nguon, "rb");
    FILE *fd = fopen(dich, "wb");
    int c;

    if (fn == NULL || fd == NULL) {
        if (fn != NULL) {
            fclose(fn);
        }
        if (fd != NULL) {
            fclose(fd);
        }
        return ERR_FILE_OPEN;
    }
    while ((c = fgetc(fn)) != EOF) {
        fputc(c, fd);
    }
    fclose(fn);
    fclose(fd);
    return SUCCESS;
}

/* Ma hoa -> giai ma -> so sanh voi tep goc. SUCCESS neu moi buoc deu dung */
static int thu_vong_tron(AlgoType algo, int che_do, const char *khoa,
                         const char *tep_goc) {
    int ma_loi;

    (void)remove(TEP_MA_HOA);
    (void)remove(TEP_GIAI_MA);
    ma_loi = chay_cipher(algo, che_do, MODE_ENCRYPT, tep_goc, TEP_MA_HOA, khoa);
    if (ma_loi != SUCCESS) {
        return ma_loi;
    }
    if (!tep_ton_tai(TEP_MA_HOA)) {
        return ERR_FILE_WRITE;
    }
    ma_loi = chay_cipher(algo, che_do, MODE_DECRYPT, TEP_MA_HOA, TEP_GIAI_MA,
                         khoa);
    if (ma_loi != SUCCESS) {
        return ma_loi;
    }
    return so_sanh_tep(tep_goc, TEP_GIAI_MA);
}

/* Thu dap an da biet: ma hoa du lieu "vao" thi phai ra dung "mong_doi" */
static int thu_dap_an(AlgoType algo, int che_do, const char *khoa,
                      const unsigned char *vao, size_t n_vao,
                      const unsigned char *mong_doi, size_t n_ra) {
    unsigned char ra[64];
    size_t da_doc;
    int ma_loi;
    FILE *f = fopen(TEP_VAO_TAM, "wb");

    if (f == NULL) {
        return ERR_FILE_OPEN;
    }
    if (fwrite(vao, 1, n_vao, f) != n_vao) {
        fclose(f);
        return ERR_FILE_WRITE;
    }
    fclose(f);

    (void)remove(TEP_MA_HOA);
    ma_loi = chay_cipher(algo, che_do, MODE_ENCRYPT, TEP_VAO_TAM, TEP_MA_HOA,
                         khoa);
    if (ma_loi != SUCCESS) {
        return ma_loi;
    }
    f = fopen(TEP_MA_HOA, "rb");
    if (f == NULL) {
        return ERR_FILE_NOT_FOUND;
    }
    da_doc = fread(ra, 1, sizeof(ra), f);
    fclose(f);
    if (da_doc != n_ra || memcmp(ra, mong_doi, n_ra) != 0) {
        return ERR_CRC_MISMATCH;
    }
    return SUCCESS;
}

/* Mong doi: chuong trinh TU CHOI, tuc la KHONG tao ra tep dau ra */
static int thu_tu_choi(AlgoType algo, int che_do, const char *khoa,
                       const char *tep_vao) {
    (void)remove(TEP_MA_HOA);
    (void)chay_cipher(algo, che_do, MODE_ENCRYPT, tep_vao, TEP_MA_HOA, khoa);
    if (tep_ton_tai(TEP_MA_HOA)) {
        return LOI_KHONG_TU_CHOI;
    }
    return SUCCESS;
}

/* Khoa gom do_dai ky tu giong nhau (do_dai = 0 la khoa rong): chuong trinh phai TU CHOI */
static int thu_khoa_dai(AlgoType algo, int che_do, char ky_tu, int do_dai) {
    char khoa[512];
    int i;

    if (do_dai < 0 || do_dai >= (int)sizeof(khoa)) {
        return ERR_INVALID_KEY;
    }
    for (i = 0; i < do_dai; i++) {
        khoa[i] = ky_tu;
    }
    khoa[do_dai] = '\0';
    return thu_tu_choi(algo, che_do, khoa, "data/8bit.bin");
}

#ifndef MENU_VU
/* Khoa Vigenere dai DUNG 255 ky tu (gioi han cua menu Huy) van phai dung duoc */
static int thu_khoa_bien_255(void) {
    char khoa[256];
    int i;

    for (i = 0; i < 255; i++) {
        khoa[i] = 'K';
    }
    khoa[255] = '\0';
    return thu_vong_tron(ALGO_VIGENERE, CHE_DO_CHU, khoa, "data/8192bit.bin");
}
#endif

/* Tep vao trung tep ra: tep goc phai con nguyen ven (khong bi xoa trang) */
static int thu_trung_tep(void) {
    int ma_loi = sao_chep_tep("data/8192bit.bin", TEP_TRUNG);

    if (ma_loi != SUCCESS) {
        return ma_loi;
    }
    (void)chay_cipher(ALGO_CAESAR, CHE_DO_BYTE, MODE_ENCRYPT, TEP_TRUNG,
                      TEP_TRUNG, "3");
    if (so_sanh_tep("data/8192bit.bin", TEP_TRUNG) != SUCCESS) {
        return ERR_FILE_WRITE;   /* tep bi hong / mat noi dung */
    }
    return SUCCESS;
}

/* Giai ma bang SAI khoa: ket qua KHONG duoc giong tep goc */
static int thu_sai_khoa(void) {
    int ma_loi;

    (void)remove(TEP_MA_HOA);
    (void)remove(TEP_GIAI_MA);
    ma_loi = chay_cipher(ALGO_CAESAR, CHE_DO_BYTE, MODE_ENCRYPT,
                         "data/8192bit.bin", TEP_MA_HOA, "3");
    if (ma_loi != SUCCESS) {
        return ma_loi;
    }
    (void)chay_cipher(ALGO_CAESAR, CHE_DO_BYTE, MODE_DECRYPT, TEP_MA_HOA,
                      TEP_GIAI_MA, "5");
    if (so_sanh_tep("data/8192bit.bin", TEP_GIAI_MA) == SUCCESS) {
        return LOI_KHONG_TU_CHOI;
    }
    return SUCCESS;
}

/* Phan 1: ma tran thuat toan x che do x tep, ma hoa roi giai ma.
   Menu Huy: Caesar, Vigenere x (byte, chu cai). Menu Vu: Caesar, Vigenere, XOR (khong co che do). */
static void chay_ma_tran(ThongKe *tk) {
    const AlgoType algo[3] = {ALGO_CAESAR, ALGO_VIGENERE, ALGO_XOR};
    const char *khoa[3] = {"3", "KHOA", "Khoa123"};   /* Vigenere cua Huy chi nhan A-Z */
    const int che_do[2] = {CHE_DO_BYTE, CHE_DO_CHU};
#ifdef MENU_VU
    const char *ten_nhom[3][2] = {{"caesar", "caesar"}, {"vigenere", "vigenere"},
                                  {"xor", "xor"}};
#else
    const char *ten_nhom[3][2] = {{"caesar-byte", "caesar-chu"},
                                  {"vigenere-byte", "vigenere-chu"},
                                  {"xor", "xor"}};
#endif
    const char *tep[SO_TEP] = {
        "data/0bit.bin",       "data/8bit.bin",       "data/56bit.bin",
        "data/64bit.bin",      "data/72bit.bin",      "data/2048bit.bin",
        "data/8192bit.bin",    "data/524280bit.bin",  "data/524288bit.bin",
        "data/524296bit.bin",  "data/tiengviet.txt",  "data/838860800bit.bin"
    };
    int i;
    int m;
    int j;

    for (i = 0; i < SO_THUAT_TOAN; i++) {
        for (m = 0; m < SO_CHE_DO; m++) {
            for (j = 0; j < SO_TEP; j++) {
                ghi_ket_qua(tk, ten_nhom[i][m], tep[j],
                            thu_vong_tron(algo[i], che_do[m], khoa[i], tep[j]));
            }
        }
    }
}

/* Phan 2: dap an da biet. Menu Vu chua ro Caesar/Vigenere o che do nao nen chi giu dap an
   dung cho moi che do (HELLO khoa 3 -> KHOOR). */
static void chay_dap_an(ThongKe *tk) {
    ghi_ket_qua(tk, "dap-an", "Caesar chu: HELLO k=3 -> KHOOR",
                thu_dap_an(ALGO_CAESAR, CHE_DO_CHU, "3",
                           (const unsigned char *)"HELLO", 5,
                           (const unsigned char *)"KHOOR", 5));
#ifndef MENU_VU
    {
        const unsigned char ff[1] = {0xFF};
        const unsigned char zero[1] = {0x00};

        ghi_ket_qua(tk, "dap-an", "Caesar chu: XYZ k=3 -> ABC",
                    thu_dap_an(ALGO_CAESAR, CHE_DO_CHU, "3",
                               (const unsigned char *)"XYZ", 3,
                               (const unsigned char *)"ABC", 3));
        ghi_ket_qua(tk, "dap-an", "Caesar byte: 0xFF k=1 -> 0x00",
                    thu_dap_an(ALGO_CAESAR, CHE_DO_BYTE, "1", ff, 1, zero, 1));
        ghi_ket_qua(tk, "dap-an", "Vigenere chu: ATTACKATDAWN LEMON",
                    thu_dap_an(ALGO_VIGENERE, CHE_DO_CHU, "LEMON",
                               (const unsigned char *)"ATTACKATDAWN", 12,
                               (const unsigned char *)"LXFOPVEFRNHR", 12));
    }
#endif
}

/* Phan 3a: khoa Caesar theo quyet dinh cua Vu: so nguyen tu 1 den 255.
   Khoa hop le (1, 255) phai ma hoa/giai ma dung. Khoa ngoai khoang phai bi TU CHOI. */
static void chay_khoa_caesar(ThongKe *tk) {
    const char *khoa_hop_le[2] = {"1", "255"};
    const char *khoa_sai[5] = {"0", "256", "300", "-1", "abc"};
    const int che_do[2] = {CHE_DO_BYTE, CHE_DO_CHU};
    const char *ten_che_do[2] = {"byte", "chu"};
    char ten[100];
    int k;
    int m;

    for (k = 0; k < 2; k++) {
        for (m = 0; m < SO_CHE_DO; m++) {
            snprintf(ten, sizeof(ten), "khoa %s (hop le), che do %s",
                     khoa_hop_le[k], ten_che_do[m]);
            ghi_ket_qua(tk, "khoa-caesar", ten,
                        thu_vong_tron(ALGO_CAESAR, che_do[m], khoa_hop_le[k],
                                      "data/8192bit.bin"));
        }
    }
    for (k = 0; k < 5; k++) {
        snprintf(ten, sizeof(ten), "khoa %s (ngoai 1-255) phai bi tu choi",
                 khoa_sai[k]);
        ghi_ket_qua(tk, "khoa-caesar", ten,
                    thu_tu_choi(ALGO_CAESAR, CHE_DO_BYTE, khoa_sai[k],
                                "data/8bit.bin"));
    }
}

/* Phan 3b: test am tinh khac */
static void chay_am_tinh(ThongKe *tk) {
    ghi_ket_qua(tk, "am-tinh", "tep vao khong ton tai",
                thu_tu_choi(ALGO_CAESAR, CHE_DO_BYTE, "3",
                            "data/khong_co.txt"));
#ifdef MENU_VU
    ghi_ket_qua(tk, "am-tinh", "khoa Vigenere rong",
                thu_khoa_dai(ALGO_VIGENERE, CHE_DO_BYTE, 'A', 0));
    ghi_ket_qua(tk, "am-tinh", "khoa XOR rong",
                thu_khoa_dai(ALGO_XOR, CHE_DO_BYTE, 'A', 0));
#else
    ghi_ket_qua(tk, "am-tinh", "khoa Vigenere sai (K3y!)",
                thu_tu_choi(ALGO_VIGENERE, CHE_DO_CHU, "K3y!",
                            "data/8bit.bin"));
    ghi_ket_qua(tk, "am-tinh", "khoa Vigenere 255 ky tu (bien hop le)",
                thu_khoa_bien_255());
#endif
    ghi_ket_qua(tk, "am-tinh", "khoa Vigenere 300 ky tu (qua dai)",
                thu_khoa_dai(ALGO_VIGENERE, CHE_DO_CHU, 'A', 300));
    ghi_ket_qua(tk, "am-tinh", "khoa Caesar 34 chu so (qua dai)",
                thu_khoa_dai(ALGO_CAESAR, CHE_DO_BYTE, '1', 34));
    ghi_ket_qua(tk, "am-tinh", "khoa Caesar 99999999999999999999 (tran so)",
                thu_tu_choi(ALGO_CAESAR, CHE_DO_BYTE, "99999999999999999999",
                            "data/8bit.bin"));
    ghi_ket_qua(tk, "am-tinh", "tep vao trung tep ra", thu_trung_tep());
    ghi_ket_qua(tk, "am-tinh", "giai ma sai khoa", thu_sai_khoa());
}

int main(void) {
    ThongKe tk;

    if (kiem_tra_cipher() != SUCCESS) {
        printf("LOI: khong chay duoc %s\n", CIPHER_CMD);
        printf("Hay dich cipher truoc, va chay run_tests tu thu muc co cipher.\n");
        return ERR_FILE_NOT_FOUND;
    }
    if (!tep_ton_tai("data/0bit.bin")) {
        printf("LOI: chua co du lieu test trong thu muc data.\n");
        printf("Hay tao thu muc data va out, roi chay gen_data truoc.\n");
        return ERR_FILE_NOT_FOUND;
    }

    tk.so_pass = 0;
    tk.so_fail = 0;
    tk.ket_qua = fopen("ketqua.txt", "w");
    if (tk.ket_qua == NULL) {
        printf("Khong tao duoc ketqua.txt\n");
        return ERR_FILE_OPEN;
    }

    chay_ma_tran(&tk);
    chay_dap_an(&tk);
    chay_khoa_caesar(&tk);
    chay_am_tinh(&tk);

    printf("=== PASS=%d  FAIL=%d ===\n", tk.so_pass, tk.so_fail);
    fprintf(tk.ket_qua, "=== PASS=%d  FAIL=%d ===\n", tk.so_pass, tk.so_fail);
    fclose(tk.ket_qua);
    return SUCCESS;
}
