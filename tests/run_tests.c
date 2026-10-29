#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_common.h"

/*
run_tests ghi san cac cau tra loi vao tep ans.tmp roi chay:
       cipher.exe < ans.tmp
   Windows: ".\\cipher.exe". Linux/WSL: dich them -DCIPHER_CMD='"./cipher"' */
#ifndef CIPHER_CMD
#define CIPHER_CMD ".\\cipher.exe"
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

/* Chay chuong trinh cua nhom: tra loi lan luot cac cau hoi cua menu.
   Thu tu cau hoi trong main.c: thuat toan, che do, tac vu, tep vao, tep ra, khoa.
   Cuoi cung "0" de thoat menu. Tra ve ma thoat cua chuong trinh (0 = tot). */
static int chay_cipher(AlgoType algo, int che_do, OpMode tac_vu,
                       const char *tep_vao, const char *tep_ra,
                       const char *khoa) {
    char lenh[600];
    FILE *tra_loi = fopen(TEP_TRA_LOI, "w");

    if (tra_loi == NULL) {
        return ERR_FILE_OPEN;
    }
    fprintf(tra_loi, "%d\n%d\n%d\n%s\n%s\n%s\n0\n", (int)algo, che_do,
            (int)tac_vu, tep_vao, tep_ra, khoa);
    fclose(tra_loi);
    snprintf(lenh, sizeof(lenh), "%s < %s > %s", CIPHER_CMD, TEP_TRA_LOI,
             TEP_NHAT_KY);
    return system(lenh);
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

/* Phan 1: ma tran thuat toan x che do x tep, ma hoa roi giai ma */
static void chay_ma_tran(ThongKe *tk) {
    const AlgoType algo[2] = {ALGO_CAESAR, ALGO_VIGENERE};
    const char *khoa[2] = {"3", "KHOA"};   /* Vigenere chi nhan chu cai A-Z */
    const int che_do[2] = {CHE_DO_BYTE, CHE_DO_CHU};
    const char *ten_nhom[2][2] = {{"caesar-byte", "caesar-chu"},
                                  {"vigenere-byte", "vigenere-chu"}};
    const char *tep[SO_TEP] = {
        "data/0bit.bin",       "data/8bit.bin",       "data/56bit.bin",
        "data/64bit.bin",      "data/72bit.bin",      "data/2048bit.bin",
        "data/8192bit.bin",    "data/524280bit.bin",  "data/524288bit.bin",
        "data/524296bit.bin",  "data/tiengviet.txt",  "data/838860800bit.bin"
    };
    int i;
    int m;
    int j;

    for (i = 0; i < 2; i++) {
        for (m = 0; m < 2; m++) {
            for (j = 0; j < SO_TEP; j++) {
                ghi_ket_qua(tk, ten_nhom[i][m], tep[j],
                            thu_vong_tron(algo[i], che_do[m], khoa[i], tep[j]));
            }
        }
    }
}

/* Phan 2: dap an da biet (vector kiem thu cua Huy) */
static void chay_dap_an(ThongKe *tk) {
    const unsigned char ff[1] = {0xFF};
    const unsigned char zero[1] = {0x00};

    ghi_ket_qua(tk, "dap-an", "Caesar chu: HELLO k=3 -> KHOOR",
                thu_dap_an(ALGO_CAESAR, CHE_DO_CHU, "3",
                           (const unsigned char *)"HELLO", 5,
                           (const unsigned char *)"KHOOR", 5));
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

/* Phan 3: test am tinh */
static void chay_am_tinh(ThongKe *tk) {
    ghi_ket_qua(tk, "am-tinh", "tep vao khong ton tai",
                thu_tu_choi(ALGO_CAESAR, CHE_DO_BYTE, "3",
                            "data/khong_co.txt"));
    ghi_ket_qua(tk, "am-tinh", "khoa Vigenere sai (K3y!)",
                thu_tu_choi(ALGO_VIGENERE, CHE_DO_CHU, "K3y!",
                            "data/8bit.bin"));
    ghi_ket_qua(tk, "am-tinh", "khoa Caesar = 0 (hop dong: 1-255)",
                thu_tu_choi(ALGO_CAESAR, CHE_DO_BYTE, "0", "data/8bit.bin"));
    ghi_ket_qua(tk, "am-tinh", "tep vao trung tep ra", thu_trung_tep());
    ghi_ket_qua(tk, "am-tinh", "giai ma sai khoa", thu_sai_khoa());
}

int main(void) {
    ThongKe tk;

    tk.so_pass = 0;
    tk.so_fail = 0;
    tk.ket_qua = fopen("ketqua.txt", "w");
    if (tk.ket_qua == NULL) {
        printf("Khong tao duoc ketqua.txt\n");
        return ERR_FILE_OPEN;
    }

    chay_ma_tran(&tk);
    chay_dap_an(&tk);
    chay_am_tinh(&tk);

    printf("=== PASS=%d  FAIL=%d ===\n", tk.so_pass, tk.so_fail);
    fprintf(tk.ket_qua, "=== PASS=%d  FAIL=%d ===\n", tk.so_pass, tk.so_fail);
    fclose(tk.ket_qua);
    return SUCCESS;
}
