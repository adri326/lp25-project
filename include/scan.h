#ifndef SCAN_H
#define SCAN_H

#include <stdbool.h>

#include <defs.h>

/**
    Scans a directory and its subdirectories, returning a `directory_t` instance.
**/
directory_t* process_dir(char* path, bool md5sum, bool verbose);
#define scan_dir process_dir

file_t* process_file(char* path, bool md5sum);

// Splits the input with `separator` and returns a pointer to the last element
const char* get_basename(const char* input, char separator);

#endif // SCAN_H
