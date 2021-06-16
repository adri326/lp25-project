#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <save.h>
#include <defs.h>

// TODO: remove root from this
bool save_to_file(directory_t* root, const char* path_to_target, const char* current_path, bool verbose) {
    FILE *output = fopen(path_to_target, "w");
    if (!output) {
        perror("ERROR: Couldn't open target file");
        fprintf(stderr, "Couldn't open %s!\n", path_to_target);
        return NULL;
    }

    if (verbose) {
        printf("Start of saving :\n");
    }

    bool res = save_to_file_recursive(output, root, 0, current_path, verbose);

    fclose(output);

    return res;
}

bool save_to_file_recursive(FILE* output, directory_t* current_dir, int depth, const char* current_path, bool verbose) {
    //Put the right number of tabulations in a string (VLA!)
    char tabulations[depth + 2];
    int i = 0;
    for (; i < depth; i++) {
        tabulations[i] = '\t';
    }
    tabulations[i] = '\0';

    //write the current directory informations
    fputs(tabulations, output);
    if (!construct_dir_line(output, *current_dir, current_path)) {
        return false;
    }
    fputs("\n", output);

    if (verbose) {
        printf("D %s%s/%s/\n", tabulations, current_path, current_dir->name);
    }

    tabulations[i] = '\t';
    tabulations[i+1] = '\0';

    //writes files
    file_t *current_file = current_dir->files;
    while (current_file != NULL) {
        fputs(tabulations, output);
        if (current_file->file_type == REGULAR_FILE) {
            if (!construct_file_line(output, *current_file, current_path)) {
                return false;
            }
            if (verbose) {
                printf("F %s%s/%s\n", tabulations, current_path, current_file->name);
            }
        } else {
            if (!construct_other_line(output, *current_file, current_path)) {
                return false;
            }
            if (verbose) {
                printf("O %s%s/%s\n", tabulations, current_path, current_file->name);
            }
        }
        fputs("\n", output);

        current_file = current_file->next_file;
    }

    //writes directories
    directory_t *subdir = current_dir->subdirs;
    while (subdir != NULL) {
        //call to recurence
        char* new_path = (char*)malloc(strlen(current_path) + strlen(subdir->name) + 2);
        if (!new_path) {
            fprintf(stderr, "Couldn't allocate %d bytes of memory!\n", (int)(strlen(current_path) + strlen(subdir->name) + 2));
            return false;
        }
        strcpy(new_path, current_path);
        strcat(new_path, "/");
        strcat(new_path, subdir->name);
        if (!save_to_file_recursive(output, subdir, depth+1, new_path, verbose)) {
            free(new_path);
            return false;
        }
        free(new_path);
        subdir = subdir->next_dir;
    }

    return true;
}

bool construct_file_line(FILE* output, file_t file, const char* path_to_parent_dir){
    char lil_buf[200] = {0};

    fputs("1\t", output); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&file.mod_time));
    fputs(lil_buf, output); //time_t

    sprintf(lil_buf, "%lu\t", file.file_size);
    fputs(lil_buf, output); //size

    for (size_t n = 0; n < MD5_DIGEST_LENGTH; n++) {
        fprintf(output, "%02hhx", file.md5sum[n]);
    }

    fputs("\t", output); //md5sum

    fprintf(output, "%s/%s", path_to_parent_dir, file.name); // path

    return true;
}

bool construct_dir_line(FILE* output, directory_t dir, const char* path_to_parent_dir){
    char lil_buf[200] = {0};

    fputs("0\t", output); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&dir.mod_time));
    fputs(lil_buf, output); //time_t

    fprintf(output, "%s/", path_to_parent_dir); // path

    return true;
}

bool construct_other_line(FILE* output, file_t file, const char* path_to_parent_dir){
    char lil_buf[200] = {0};

    fputs("2\t", output); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&file.mod_time));
    fputs(lil_buf, output); //time_t

    fprintf(output, "%s/%s", path_to_parent_dir, file.name); // path

    return true;
}
