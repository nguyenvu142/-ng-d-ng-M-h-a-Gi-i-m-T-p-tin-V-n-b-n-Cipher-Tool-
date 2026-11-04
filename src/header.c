#include <stdio.h>     // Dành cho kiểu FILE
#include <stdint.h>    // Dành cho các kiểu uint16_t, uint32_t, uint64_t
#include "common.h"    // (Dự đoán) Chứa định nghĩa kiểu ErrCode
#include "fileinfo.h"  // (Dự đoán) Chứa định nghĩa struct Header
#include "cipher.h"    // Tùy chọn: Include thêm nếu dùng hàm/macro từ đây

// Các hằng số (như trong ảnh của bạn)
#define MAGIC0 'C'
#define MAGIC1 'T'
#define MAGIC2 'L'
#define MAGIC3 '1'

void put_u16_le(unsigned char *p, uint16_t v) {
    p[0] = (unsigned char)(v & 0xFFu);
    p[1] = (unsigned char)((v >> 8) & 0xFFu);
}

void put_u32_le(unsigned char *p, uint32_t v) {
    int i;
    for (i = 0; i < 4; i++) {
        p[i] = (unsigned char)((v >> (8 * i)) & 0xFFu);
    }
}

void put_u64_le(unsigned char *p, uint64_t v) {
    int i;
    for (i = 0; i < 8; i++) {
        p[i] = (unsigned char)((v >> (8 * i)) & 0xFFu);
    }
}

ErrCode header_write(FILE *f, const Header *h) {
    // ... [Code xử lý logic ghi header của bạn nằm ở đây] ...

    // Khắc phục lỗi "control reaches end of non-void function"
    return 0; // Hoặc return SUCCESS; (tùy theo cách định nghĩa ErrCode trong dự án)
}

ErrCode header_read(FILE *f, Header *h) {
    // ... [Code xử lý logic đọc header của bạn nằm ở đây] ...

    return 0; 
}
