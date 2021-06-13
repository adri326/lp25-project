#ifndef SAVE_H
#define SAVE_H

#include "defs.h"
#include <stdbool.h>

int save_to_file(directory_t *root, char *path_to_target, int depth, char *path_to_current_dir, bool verbose);

int construct_file_line(char *buffer, file_t file, char *path_to_parent_dir);

int construct_dir_line(char *buffer, directory_t dir, char *path_to_parent_dir);

int construct_other_line(char *buffer, file_t file, char *path_to_parent_dir);

#endif // SAVE_H
