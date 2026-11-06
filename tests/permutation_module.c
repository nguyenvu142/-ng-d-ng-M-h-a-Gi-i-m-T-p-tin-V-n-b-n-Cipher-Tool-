#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef PERM_MODULE_BLOCK_SIZE
#define PERM_MODULE_BLOCK_SIZE 8
#endif

void perm_from_key(const uint8_t *key, size_t key_len, uint8_t *perm) {
    if (perm == NULL) {
        return;
    }

    for (size_t i = 0; i < PERM_MODULE_BLOCK_SIZE; i++) {
        perm[i] = (uint8_t)i;
    }

    if (key == NULL || key_len == 0) {
        return;
    }

    typedef struct {
        uint8_t val;
        size_t orig_idx;
    } KeyElem;

    KeyElem elems[PERM_MODULE_BLOCK_SIZE];
    for (size_t i = 0; i < PERM_MODULE_BLOCK_SIZE; i++) {
        elems[i].val = key[i % key_len];
        elems[i].orig_idx = i;
    }

    for (size_t i = 0; i < PERM_MODULE_BLOCK_SIZE - 1; i++) {
        for (size_t j = i + 1; j < PERM_MODULE_BLOCK_SIZE; j++) {
            if (elems[j].val < elems[i].val || 
               (elems[j].val == elems[i].val && elems[j].orig_idx < elems[i].orig_idx)) {
                KeyElem tmp = elems[i];
                elems[i] = elems[j];
                elems[j] = tmp;
            }
        }
    }

    for (size_t i = 0; i < PERM_MODULE_BLOCK_SIZE; i++) {
        perm[i] = (uint8_t)elems[i].orig_idx;
    }
}
