#include <tree.h>
#include <assert.h>
#include <stdlib.h>

bool append_subdir(directory_t* child, directory_t* parent) {
    assert(parent != NULL);

    if (parent->subdirs) {
        directory_t* tmp = parent->subdirs;
        LL_TAIL(tmp, next_dir);
        tmp->next_dir = child;
    } else {
        parent->subdirs = child;
    }

    return true;
}

bool append_file(file_t* child, directory_t* parent) {
    assert(parent != NULL);

    if (parent->files) {
        file_t* tmp = parent->files;
        LL_TAIL(tmp, next_file);
        tmp->next_file = child;
    } else {
        parent->files = child;
    }

    return true;
}

void clear_files(directory_t* parent) {
    if (parent == NULL) return;

    file_t* tmp = parent->files;
    while (tmp) {
        file_t* next = tmp->next_file;
        free(tmp);
        tmp = next;
    }

    parent->files = NULL;
}

void clear_subdirs(directory_t* parent) {
    if (parent == NULL) return;

    free_dir(parent->subdirs);
    parent->subdirs = NULL;
}

void free_dir(directory_t* dir) {
    directory_t* tmp = dir;
    while (tmp) {
        clear_files(tmp);
        free_dir(tmp->subdirs);
        directory_t* next = tmp->next_dir;
        free(tmp);
        tmp = next;
    }
}
