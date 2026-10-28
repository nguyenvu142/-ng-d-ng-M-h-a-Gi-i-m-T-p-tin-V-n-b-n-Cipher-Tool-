#ifndef TEST_COMMON_H
#define TEST_COMMON_H

/* Ma loi theo quy uoc chung cua nhom.
   Khi nhom co common.h chinh thuc, xoa file nay va doi #include. */
#define SUCCESS              0
#define ERR_OK               0
#define ERR_FILE_NOT_FOUND  (-1)
#define ERR_FILE_OPEN       (-2)
#define ERR_FILE_READ       (-3)
#define ERR_FILE_WRITE      (-4)
#define ERR_INVALID_KEY     (-5)
#define ERR_INVALID_ALGO    (-6)
#define ERR_INVALID_MODE    (-7)
#define ERR_CRC_MISMATCH    (-8)
#define ERR_INVALID_PADDING (-9)
#define ERR_MEM_ALLOC       (-10)

/* Thong so da chot cua nhom */
#define BLOCK_SIZE    65536   /* bo dem doc tep */
#define PERM_BLOCK_N  8       /* khoi hoan vi mac dinh */

typedef enum {
    ALGO_CAESAR = 1,
    ALGO_VIGENERE = 2,
    ALGO_XOR = 3,
    ALGO_TRANSPOSITION = 4
} AlgoType;

typedef enum {
    MODE_ENCRYPT = 1,
    MODE_DECRYPT = 2
} OpMode;

#endif
