#include "permutation.h"

void permute_block(const char *input_block, const int *key, char *output_block, int N) {
    for (int i = 0; i < N; i++) {
        output_block[key[i]] = input_block[i];
    }
}

PermStatus process_permutation(const char *input_path, const char *output_path, const int *key, int N) {
    FILE *in_file = fopen(input_path, "rb");
    if (!in_file) {
        return PERM_ERROR_FILE_NOT_FOUND;
    }

    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    if (file_size == 0) {
        FILE *out_file = fopen(output_path, "wb");
        if (out_file) fclose(out_file);
        fclose(in_file);
        return PERM_ERROR_EMPTY_FILE; 
    }

    if (file_size % N != 0) {
        fclose(in_file);
        return PERM_ERROR_NOT_DIVISIBLE;
    }

    FILE *out_file = fopen(output_path, "wb");
    if (!out_file) {
        fclose(in_file);
        return PERM_ERROR_FILE_NOT_FOUND;
    }

    char *input_block = (char *)malloc(N * sizeof(char));
    char *output_block = (char *)malloc(N * sizeof(char));

    while (fread(input_block, sizeof(char), N, in_file) == N) {
        permute_block(input_block, key, output_block, N);
        fwrite(output_block, sizeof(char), N, out_file);
    }

    free(input_block);
    free(output_block);
    fclose(in_file);
    fclose(out_file);

    return PERM_SUCCESS;
}
