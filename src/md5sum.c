#include <md5sum.h>
#include <stdio.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 16

bool compute_md5(char *path, uint8_t buffer[]) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Couldn't open %s!\n", path);
        return false;
    }
    MD5_CTX ctx;

    MD5_Init(&ctx);
    uint8_t tmp_buffer[BUFFER_SIZE] = {0};

    while (!feof(file)) {
        size_t count = fread(tmp_buffer, 1, BUFFER_SIZE, file);
        if (count == 0) break;
        MD5_Update(&ctx, tmp_buffer, count);
    }

    if (ferror(file)) {
        fprintf(stderr, "Error while reading %s!\n", path);
        fclose(file);
        return false;
    } else {
        MD5_Final(buffer, &ctx);
        return true;
    }
}
