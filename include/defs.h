#ifndef DEFS_H
#define DEFS_H

#include <linux/limits.h>
#include <time.h>
#include <openssl/md5.h>
#include <inttypes.h>

enum ftype {
    DIRECTORY,
    REGULAR_FILE,
    OTHER_TYPE
};
typedef enum ftype ftype_t;

struct file {
    ftype_t file_type;
    char name[NAME_MAX+1];
    time_t mod_time;
    uint64_t file_size;
    uint8_t md5sum[MD5_DIGEST_LENGTH];
    struct file* next_file;
};

typedef struct file file_t;

struct directory {
    char name[NAME_MAX+1];
    time_t mod_time;
    struct directory* subdirs;
    file_t* files;
    struct directory* next_dir;
};

typedef struct directory directory_t;

/**
    Sets `ll` to the last element of the linked list.
    `ll` must initially be set to one of the lements of the linked list.
    If `ll` is NULL, it is left untouched.
    Assumes that (ll, next) is a non-cyclic linked list, where ll->next is a reference to the next element of the linked list.
**/
#define LL_TAIL(ll, next) if (ll) while ((ll)->next) ll = (ll)->next;

#endif // DEFS_H
