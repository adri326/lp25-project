#include <string.h>
#include <stdio.h>

#include <save.h>
#include <defs.h>


int save_to_file(directory_t *root, char *path_to_target, int depth, char *path_to_current_dir) {

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
    fputs(tabulations, f);
		construct_dir_line(buffer, current_dir, path_to_current_dir);
		fputs(buffer, f);
		tabulations[i] = '\t';
		tabulations[i+1] = '\0';

		//writes files
		file_t *current_file = root.files;
		char buffer[200];
		while (current_file != null) {
			if (current_file.file_type == 1) {
				construct_file_line(buffer, current, path_to_current_dir);
			}
			else {
				construct_other_line(buffer, current, path_to_current_dir);		
			}
			fputs(tabulations, f);
			fputs(buffer, f);
			current_file = current_file->next_file;
		}

		//writes directories
		directory_t *current_dir = root.subdirs;
		char next_dir_path[200];
		while (current_dir != null) {
			//call to recurence
			strcpy(next_dir_path, path_to_current_dir);
			strcat(next_dir_path, "/");
			strcat(next_dir_path, current_dir->name);
			save_to_file(current_dir, path_to_target, depth+1, next_dir_path);
			current_dir = current_dir->next_dir;
		}


    return 0;
}






int construct_file_line(char *buffer, file_t file, char *path_to_parent_dir){

    char str_time[20];
    strftime(str_time, 20, "%Y-%m-%d %H:%M:%S", file.mod_time);

    strcpy(buffer, atoi(file.file_type)); //e_type
    strcat(buffer, "\t");

    strcat(buffer, str_time); //time_t
    strcat(buffer, "\t");

    strcat(buffer, atoi(file.file_sise)); //size
    strcat(buffer, "\t");

    strcat(buffer, path_to_parent_dir); //begining of path
    strcat(buffer, file.name);
    strcat(buffer, "/");

    return 0;
}

int construct_dir_line(char *buffer, directory_t dir, char *path_to_parent_dir){

    char str_time[20];
    strftime(str_time, 20, "%Y-%m-%d %H:%M:%S", dir.mod_time);

    strcpy(buffer, atoi(e_type.DIRECTORY)); //e_type
    strcat(buffer, "\t");

    strcat(buffer, str_time); //time_t
    strcat(buffer, "\t");

    strcat(buffer, path_to_parent_dir); //begining of path
    strcat(buffer, dir.name);
    strcat(buffer, "/");

    return 0;
}

int construct_other_line(char *buffer, file_t file, char *path_to_parent_dir){

    char str_time[20];
    strftime(str_time, 20, "%Y-%m-%d %H:%M:%S", file.mod_time);

    strcpy(buffer, atoi(file.file_type)); //e_type
    strcat(buffer, "\t");

    strcat(buffer, str_time); //time_t
    strcat(buffer, "\t");

    strcat(buffer, path_to_parent_dir); //begining of path
    strcat(buffer, file.name);
    strcat(buffer, "/");

    return 0;
}

