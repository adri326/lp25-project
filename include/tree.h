#ifndef TREE_H
#define TREE_H

#include "defs.h"
#include <stdbool.h>

/**
    Appends `child` to the end of `parent->subdirs`.
    Asserts that `parent != NULL`.
    Assumes non-cyclicity.
**/
bool append_subdir(directory_t* child, directory_t* parent);

/**
    Appends `child` to the end of `parent->files`.
    Asserts that `parent != NULL`.
    Assumes non-cyclicity.
**/
bool append_file(file_t* child, directory_t* parent);

/**
    Frees the list of files in `parent`.
    Assumes non-cyclicity.
**/
void clear_files(directory_t* parent);

/**
    Equivalent to calling `free_dir(parent->subdirs)`.
    Sets `parent->subdirs` to NULL.
**/
void clear_subdirs(directory_t* parent);

/**
    Frees `dir`, its subdirectories and the next files in the linked list.
    Assumes non-cyclicity.
**/
void free_dir(directory_t* dir);

#endif // TREE_H
