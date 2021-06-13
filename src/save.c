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
    char tabulations[depth+2];
    int i;
    for (i=0; i<depth; i++) {
        tabulations[i] = '\t';
    }
    tabulations[i] = '\0';

    //write the current directory informations
    // char buffer[200] = {0};
    fputs(tabulations, output);
    construct_dir_line(output, *current_dir, current_path);
    // fputs(buffer, output);
    fputs("\n", output);

    if (verbose) {
        printf("D %s%s\n", tabulations, current_dir->name);
    }

    tabulations[i] = '\t';
    tabulations[i+1] = '\0';

    //writes files
    file_t *current_file = current_dir->files;
    while (current_file != NULL) {
        fputs(tabulations, output);
        if (current_file->file_type == REGULAR_FILE) {
            construct_file_line(output, *current_file, current_path);
            if (verbose) {
                printf("F %s%s\n", tabulations, current_file->name);
            }
        } else {
            construct_other_line(output, *current_file, current_path);
            if (verbose) {
                printf("O %s%s\n", tabulations, current_file->name);
            }
        }
        // fputs(buffer, output);
        fputs("\n", output);



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

    fprintf(output, "%s/%s/", path_to_parent_dir, dir.name); // path

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
