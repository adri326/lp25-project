#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <save.h>
#include <defs.h>

// TODO: remove root from this
bool save_to_file(directory_t* root, const char* path_to_target, int depth, const char* current_path, bool verbose) {
    FILE *output = fopen(path_to_target, "w");
    if (!output) {
        perror("ERROR: Couldn't open target file");
        fprintf(stderr, "Couldn't open %s!\n", path_to_target);
        return NULL;
    }

    if (verbose) {
        printf("Start of saving :\n");
    }

    bool res = save_to_file_recursive(output, root, depth, current_path, verbose);

    fclose(output);

    return res;
}

bool save_to_file_recursive(FILE* output, directory_t* current_dir, int depth, const char* current_path, bool verbose) {
    //Put the right number of tabulations in a string (VLA!)
    char tabulations[depth+2];
    int i;
    for (i=0; i<depth; i++) {
        tabulations[i] = '\t';
    }
    tabulations[i] = '\0';

    //write the current directory informations
    char buffer[200] = {0};
    fputs(tabulations, output);
    construct_dir_line(buffer, *current_dir, current_path);
    fputs(buffer, output);
    fputs("\n", output);

    if (verbose) {
        printf("%s%s\n", tabulations, buffer);
    }

    tabulations[i] = '\t';
    tabulations[i+1] = '\0';

    //writes files
    file_t *current_file = current_dir->files;
    while (current_file != NULL) {
        if (current_file->file_type == REGULAR_FILE) {
            construct_file_line(buffer, *current_file, current_path);
        } else {
            construct_other_line(buffer, *current_file, current_path);
        }
        fputs(tabulations, output);
        fputs(buffer, output);
        fputs("\n", output);

        if (verbose) {printf("%s%s\n", tabulations, buffer);}

        current_file = current_file->next_file;
    }

    //writes directories
    directory_t *subdir = current_dir->subdirs;
    while (subdir != NULL) {
        //call to recurence
        char* new_path = (char*)malloc(strlen(current_path) + strlen(subdir->name) + 2);
        strcpy(new_path, current_path);
        strcat(new_path, "/");
        strcat(new_path, subdir->name);
        save_to_file_recursive(output, subdir, depth+1, new_path, verbose);
        subdir = subdir->next_dir;
    }

    return 0;
}

bool construct_file_line(char* buffer, file_t file, const char* path_to_parent_dir){
    char lil_buf[200] = {0};

    strcpy(buffer, "1\t"); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&file.mod_time));
    strcat(buffer, lil_buf); //time_t

    sprintf(lil_buf, "%lu\t", file.file_size);
    strcat(buffer, lil_buf); //size

    for (size_t n = 0; n < MD5_DIGEST_LENGTH; n++) {
        sprintf(lil_buf, "%02hhx", file.md5sum[n]);
        strcat(buffer, lil_buf);
    }

    sprintf(lil_buf, "\t");
    strcat(buffer, lil_buf); //md5sum

    strcat(buffer, path_to_parent_dir); //benigging of path
    strcat(buffer, "/");
    strcat(buffer, file.name);

    return 0;
}

bool construct_dir_line(char* buffer, directory_t dir, const char* path_to_parent_dir){
    char lil_buf[200] = {0};

    strcpy(buffer, "0\t"); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&dir.mod_time));
    strcat(buffer, lil_buf); //time_t

    strcat(buffer, path_to_parent_dir); //begining of path
    strcat(buffer, "/");
    strcat(buffer, dir.name);
    strcat(buffer, "/");

    return 0;
}

bool construct_other_line(char* buffer, file_t file, const char* path_to_parent_dir){
    char lil_buf[200] = {0};

    strcpy(buffer, "2\t"); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&file.mod_time));
    strcat(buffer, lil_buf); //time_t

    strcat(buffer, path_to_parent_dir); //begining of path
    strcat(buffer, "/");
    strcat(buffer, file.name);

    return 0;
}
