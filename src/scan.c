#include <scan.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <assert.h>
#include <defs.h>
#include <tree.h>
#include <md5sum.h>


directory_t* process_dir(char* path, bool md5sum, bool verbose) {
    directory_t* root = (directory_t*)malloc(sizeof(directory_t));
    root->files = NULL;
    root->subdirs = NULL;
    root->next_dir = NULL;

    struct stat stat_buffer;
    stat(path, &stat_buffer);

    //mod_time
    root->mod_time = stat_buffer.st_mtime;

    //name
    const char* base = get_basename(path, '/');
    strcpy(root->name, base);

    struct dirent* file = NULL;
    DIR* dir = opendir(path);
    if (!dir)
    {
        perror("ERROR while opening directory");
        fprintf(stderr, "Couldn't open directory '%s'\n", path);
        return NULL;
    }

    //files and sub dirs
    if (verbose) {
        printf("%s\n", path);
    }
    file = readdir(dir);
    while (file != NULL) {
        //file_t* f = NULL;
        if (file->d_name[0] != '.') {
            char* str_buffer = (char*)malloc(strlen(path) + strlen(file->d_name) + 2);
            strcpy(str_buffer, path);
            strcat(str_buffer, "/");
            strcat(str_buffer, file->d_name);
            if (verbose) {printf("%s\n", str_buffer);}

            append_file(process_file(str_buffer, md5sum), root);

            if ((int)file->d_type == 4) {
                directory_t* newDir = process_dir(str_buffer, md5sum, verbose);
                append_subdir(newDir, root);
            }
            free(str_buffer);
        }
        file = readdir(dir);
    }

    if (dir) {
        closedir(dir);
    }
    return root;
}

file_t* process_file(char* path, bool md5sum) {
    file_t* files = (file_t*)malloc(sizeof(file_t));
    files->next_file = NULL;

    const char* base = get_basename(path, '/');
    strcpy(files->name, base);

    struct stat stat_buffer;
    stat(path, &stat_buffer);

    if (S_ISDIR(stat_buffer.st_mode)) {
        files->file_type = DIRECTORY;
    } else if (S_ISREG(stat_buffer.st_mode)) {
        files->file_type = REGULAR_FILE;
        if (md5sum) {
            compute_md5(path, files->md5sum);
        }
    } else {
        files->file_type = OTHER_TYPE;
        if (md5sum) {
            compute_md5(path, files->md5sum);
        }
    }

    files->file_size = stat_buffer.st_size;
    files->mod_time = stat_buffer.st_mtime;

    return files;
}

const char* get_basename(const char* input, char separator) {
    assert(separator != 0);
    int length = strlen(input);
    int i = length-1;

    while (i >= 0) {
        if (input[i] == separator) {
            return &input[i + 1];
        }
        i--;
    }

    return NULL;
}
