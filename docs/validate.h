#ifndef VALIDATE_H
#define VALIDATE_H
#include <stdbool.h>

/* Chuyen chuoi nhap vao thanh so nguyen. Tra ve false neu khong phai so. */
bool parse_caesar_key(const char *text, int *key);

/* Khoa Caesar hop le: so nguyen tu 1 den 255. Tra ve false neu ngoai khoang.
   In canh bao neu khoa khong lam thay doi du lieu (vi du 26 o che do chu cai). */
bool validate_caesar_key(int key, bool byte_mode);
bool validate_vigenere_key(const char *key);
#endif
