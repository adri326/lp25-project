#ifndef SCAN_H
#define SCAN_H

#include <stdbool.h>

#include <defs.h>


directory_t *process_dir(char *path, bool md5sum, bool verbose);
file_t *process_file(char *path, bool md5sum);


//returns the last string in the splited input
int last_of_split(char *inpt, char split_character, char *outpt);


#endif // SCAN_H
