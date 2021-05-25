#include <md5sum.h>
#include <stdio.h>
#ifdef OPENSSL_MD5
    #include <openssl/md5.h>
    #define CTX MD5_CTX
    #define INIT MD5_Init
    #define UPDATE MD5_Update
    #define FINAL MD5_Final
#else
    #ifdef BSD_MD5
        #include <bsd/md5.h>
        #define CTX MD5_CTX
        #define INIT MD5Init
        #define UPDATE MD5Update
        #define FINAL MD5Final
    #else
        #error No MD5 library to use!
    #endif
#endif

#define BUFFER_SIZE 16

bool compute_md5(char *path, uint8_t buffer[]) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Couldn't open %s!\n", path);
        return false;
    }
    CTX ctx;

    INIT(&ctx);
    uint8_t tmp_buffer[BUFFER_SIZE] = {0};

    while (!feof(file)) {
        size_t count = fread(tmp_buffer, 1, BUFFER_SIZE, file);
        if (count == 0) break;
        UPDATE(&ctx, tmp_buffer, count);
    }

    if (ferror(file)) {
        fprintf(stderr, "Error while reading %s!\n", path);
        fclose(file);
        return false;
    } else {
        FINAL(buffer, &ctx);
        return true;
    }
}
