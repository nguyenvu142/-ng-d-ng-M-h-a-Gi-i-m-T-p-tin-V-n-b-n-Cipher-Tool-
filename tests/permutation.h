#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    PERM_SUCCESS = 0,
    PERM_ERROR_FILE_NOT_FOUND = 1,
    PERM_ERROR_NOT_DIVISIBLE = 2,
    PERM_ERROR_EMPTY_FILE = 3
} PermStatus;

void permute_block(const char *input_block, const int *key, char *output_block, int N);
PermStatus process_permutation(const char *input_path, const char *output_path, const int *key, int N);

#endif
