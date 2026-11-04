#include <stdio.h>
#include "cipher.h"

int main(void) {
    int choice;
    printf("=== CHUONG TRINH MA HOA / GIAI MA TAP TIN (MOC 2 - STUB) ===\n");
    printf("1. Ma hoa / Giai ma Caesar (Stub)\n");
    printf("2. Ma hoa / Giai ma Vigenere (Stub)\n");
    printf("3. Ma hoa / Giai ma XOR (Stub)\n");
    printf("Chon chuc nang (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Loi nhap lieu!\n");
        return 1;
    }
    
    printf("Ban da chon chuc nang: %d. Dang goi stub xu ly...\n", choice);
    return 0;
}
