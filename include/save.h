#ifndef SAVE_H
#define SAVE_H

#include "defs.h"
#include <stdbool.h>

bool save_to_file(directory_t* root, const char* path_to_target, int depth, const char* current_path, bool verbose);

bool save_to_file_recursive(FILE* output, directory_t* current_dir, int depth, const char* current_path, bool verbose);

bool construct_file_line(char* buffer, file_t file, const char* path_to_parent_dir);

bool construct_dir_line(char* buffer, directory_t dir, const char* path_to_parent_dir);

bool construct_other_line(char* buffer, file_t file, const char* path_to_parent_dir);

#endif // SAVE_H
