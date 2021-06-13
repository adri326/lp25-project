#ifndef SCAN_H
#define SCAN_H

#include <stdbool.h>

#include <defs.h>

directory_t* process_dir(char* path, bool md5sum, bool verbose);
file_t* process_file(char* path, bool md5sum);

// Splits the input with `separator` and returns a pointer to the last element
const char* get_basename(const char* input, char separator);

#endif // SCAN_H
