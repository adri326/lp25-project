#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <save.h>
#include <defs.h>


int save_to_file(directory_t *root, char *path_to_target, int depth, char *path_to_current_dir, bool verbose) {

    //empties the target file if we are at depth 0
    if (depth == 0) {
        FILE *w = fopen(path_to_target, "w");
        fclose(w);
        if (verbose) {printf("Start of saving :\n");}
    }

    //Put the right number of tabulations in a string

    char tabulations[depth+2];
    tabulations[0] = '\0';
    int i;
    for (i=0; i<depth; i++) {
        tabulations[i] = '\t';
    }
    tabulations[i] = '\0';

    //open file
    FILE *f = fopen(path_to_target, "a");
    if (!f){return 0;}

    //write the current directory informations
    char buffer[200] = {0};
    fputs(tabulations, f);
    construct_dir_line(buffer, *root, path_to_current_dir);
    fputs(buffer, f);
    fputs("\n", f);
    
    if (verbose) {printf("%s%s\n", tabulations, buffer);}
    
    tabulations[i] = '\t';
    tabulations[i+1] = '\0';

    //writes files
    file_t *current_file = root->files;
    while (current_file != NULL) {
        if (current_file->file_type == REGULAR_FILE) {
            construct_file_line(buffer, *current_file, path_to_current_dir);
        }
        else {
            construct_other_line(buffer, *current_file, path_to_current_dir);
        }
        fputs(tabulations, f);
        fputs(buffer, f);
        fputs("\n", f);
        
        if (verbose) {printf("%s%s\n", tabulations, buffer);}
        
        current_file = current_file->next_file;
    }

    fclose(f);

    //writes directories
    directory_t *current_dir = root->subdirs;
    char next_dir_path[200] = {0};
    while (current_dir != NULL) {
        //call to recurence
        strcpy(next_dir_path, path_to_current_dir);
        strcat(next_dir_path, "/");
        strcat(next_dir_path, current_dir->name);
        save_to_file(current_dir, path_to_target, depth+1, next_dir_path, verbose);
        current_dir = current_dir->next_dir;
    }


    return 0;
}






int construct_file_line(char *buffer, file_t file, char *path_to_parent_dir){

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

int construct_dir_line(char *buffer, directory_t dir, char *path_to_parent_dir){

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

int construct_other_line(char *buffer, file_t file, char *path_to_parent_dir){

    char lil_buf[200] = {0};

    strcpy(buffer, "2\t"); //e_type

    strftime(lil_buf, 200, "%Y-%m-%d %H:%M:%S\t", localtime(&file.mod_time));
    strcat(buffer, lil_buf); //time_t

    strcat(buffer, path_to_parent_dir); //begining of path
    strcat(buffer, "/");
    strcat(buffer, file.name);

    return 0;
}
