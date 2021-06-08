#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	char buffer[200];
	fputs(tabulations, f);
	construct_dir_line(buffer, *root, path_to_current_dir);
	fputs(buffer, f);
	tabulations[i] = '\t';
	tabulations[i+1] = '\0';

	//writes files
	file_t *current_file = root->files;
	while (current_file != NULL) {
		if (current_file->file_type == 1) {
			construct_file_line(buffer, *current_file, path_to_current_dir);
		}
		else {
			construct_other_line(buffer, *current_file, path_to_current_dir);		
		}
		fputs(tabulations, f);
		fputs(buffer, f);
		current_file = current_file->next_file;
	}

	//writes directories
	directory_t *current_dir = root->subdirs;
	char next_dir_path[200];
	while (current_dir != NULL) {
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

	char lil_buf[20];

	strcpy(buffer, "1"); //e_type
	strcat(buffer, "\t");

	strftime(lil_buf, 20, "%Y-%m-%d %H:%M:%S", localtime(&file.mod_time));
	strcat(buffer, lil_buf); //time_t
	strcat(buffer, "\t");

	sprintf(lil_buf, "%lu", file.file_size); 
	strcat(buffer, lil_buf); //size
	strcat(buffer, "\t");

	strcat(buffer, path_to_parent_dir); //begining of path
	strcat(buffer, file.name);
	strcat(buffer, "/");

	return 0;
}

int construct_dir_line(char *buffer, directory_t dir, char *path_to_parent_dir){

	char lil_buf[20];

	strcpy(buffer, "0"); //e_type
	strcat(buffer, "\t");

	strftime(lil_buf, 20, "%Y-%m-%d %H:%M:%S", localtime(&dir.mod_time));
	strcat(buffer, lil_buf); //time_t
	strcat(buffer, "\t");

	strcat(buffer, path_to_parent_dir); //begining of path
	strcat(buffer, dir.name);
	strcat(buffer, "/");

	return 0;
}

int construct_other_line(char *buffer, file_t file, char *path_to_parent_dir){

	char lil_buf[20];

	strcpy(buffer, "2"); //e_type
	strcat(buffer, "\t");

	strftime(lil_buf, 20, "%Y-%m-%d %H:%M:%S", localtime(&file.mod_time));
	strcat(buffer, lil_buf); //time_t
	strcat(buffer, "\t");

	strcat(buffer, path_to_parent_dir); //begining of path
	strcat(buffer, file.name);
	strcat(buffer, "/");

	return 0;
}

