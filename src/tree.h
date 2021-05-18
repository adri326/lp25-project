#ifndef TREE_H
#define TREE_H

#include "defs.h"
#include <stdbool.h>

bool append_subdir(directory_t *child, directory_t *parent);
bool append_file(file_t *child, directory_t *parent);
void clear_files(directory_t *parent);
void clear_subdirs(directory_t *parent);

#endif // TREE_H
