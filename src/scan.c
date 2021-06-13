#include <scan.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <defs.h>
#include <tree.h>
#include <md5sum.h>



directory_t *process_dir(char *path) {
    directory_t* root = (directory_t*)malloc(sizeof(directory_t));
    root->files = NULL;
    root->subdirs = NULL;
    root->next_dir = NULL;
  
    struct stat stat_buffer;// = malloc(sizeof(struct stat));
    stat(path, &stat_buffer);
  
    //mod_time
    root->mod_time = stat_buffer.st_mtime;

    //name
    char str_buffer[200];
    last_of_split(path, '/', str_buffer);
    strcpy(root->name, str_buffer);
    
    
    struct dirent* file = NULL;
    DIR* dir = opendir(path);
    if(!dir)
    {
        perror("ERROR");
        return NULL;
    }

    //files and sub dirs
    file = readdir(dir);
    while (file != NULL) {
        //file_t* f = NULL;
        if (file->d_name[0] != '.') {
            strcpy(str_buffer, path);
            strcat(str_buffer, "/");
            strcat(str_buffer, file->d_name);
            
            append_file(process_file(str_buffer), root);

            if ((int)file->d_type == 4) {
                directory_t* newDir = process_dir(str_buffer);
                append_subdir(newDir,root);
            }
            
        }

    }

    if (dir) {
        closedir(dir);
    }
    return root;


}




file_t *process_file(char *path) {
    file_t* files = (file_t*)malloc(sizeof(file_t));
    files->next_file = NULL;

    char str_buffer[200];
    last_of_split(path, '/', str_buffer);
    strcpy(files->name, str_buffer);

    struct stat stat_buffer;
    stat(path, &stat_buffer);

    if (S_ISDIR(stat_buffer.st_mode)) {
        files->file_type = DIRECTORY;
    }
    else if (S_ISREG(stat_buffer.st_mode)) {
        files->file_type = REGULAR_FILE;
        compute_md5(path, files->md5sum);
    }
    else {
        files->file_type = OTHER_TYPE;
        compute_md5(path, files->md5sum);
    }


    files->file_size = stat_buffer.st_size;
    files->mod_time = stat_buffer.st_mtime;

    return files;


}




int last_of_split(char *inpt, char split_character, char *outpt) {

    int length = strlen(inpt);
    int i = length-1;
    while (i >= 0 && inpt[i] != split_character) {
        i--;
    }
    
    outpt[0] = '\0';
    int head = 0;
    i++;
    while (i < length) {
        outpt[head] = inpt[i];
        i++;
        head++;
    }
    outpt[head] = '\0';


    return 0;
}
