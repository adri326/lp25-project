#ifndef MD5SUM_H
#define MD5SUM_H

#include <stdbool.h>
#include <inttypes.h>

bool compute_md5(char *path, uint8_t buffer[]);

#endif // MD5SUM_H
