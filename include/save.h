#ifndef SAVE_H
#define SAVE_H

#include "defs.h"
#include <stdbool.h>

/**
    Writes the contents of `root` into the file with as path `path_to_target`.
**/
bool save_to_file(directory_t* root, const char* path_to_target, const char* current_path, bool verbose);

/**
    Internal; recursive function to save the contents of `current_dir` into the output file `output`.
    `depth` controls the tabulation amount and `current_path` will be prepended to the file paths.
**/
bool save_to_file_recursive(FILE* output, directory_t* current_dir, int depth, const char* current_path, bool verbose);

/**
    Internal; writes a single regular file line to `output`.
    `path_to_parent_dir` will be pretended to the file's basename.
**/
bool construct_file_line(FILE* output, file_t file, const char* path_to_parent_dir);

/**
    Internal; writes a single directory line to `output`.
    `path_to_parent_dir` will be pretended to the directory's basename.
**/
bool construct_dir_line(FILE* output, directory_t dir, const char* path_to_parent_dir);

/**
    Internal; writes a single other file line to `output`.
    `path_to_parent_dir` will be pretended to the file's basename.
**/
bool construct_other_line(FILE* output, file_t file, const char* path_to_parent_dir);

#endif // SAVE_H
