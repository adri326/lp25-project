#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include <tree.h>
#include <scan.h>
#include <save.h>

int main(int argc, char* argv[]) {
    const char* home_dir = getenv("HOME");

    char* output_path = NULL;
    // Build the default value for output_path
    {
        // Date
        char date[200] = "";
        time_t now = time(0);
        struct tm* t = localtime(&now);
        strftime(date, 199, "%Y-%m-%d-%H:%M:%S", t);

        output_path = malloc(strlen(home_dir) + strlen(date) + 20);
        sprintf(output_path, "%s/.filescanner/%s.scan", home_dir, date);

        // Create ~/.filescanner

        char* output_dir = malloc(strlen(home_dir) + 14);
        sprintf(output_dir, "%s/.filescanner", home_dir);
        DIR* dir = opendir(output_dir);
        if (!dir) {
            mkdir(output_dir, 0744);
        }
        closedir(dir);
        free(output_dir);
    }

    char* input_path = malloc(2);
    sprintf(input_path, ".");
    bool md5sum = false;
    bool verbose = false;

    //getopt
    int opt = 0;
    while((opt = getopt(argc, argv, "i:so:v")) != -1) {
        switch (opt) {
            case 'i':
                /*'i' as in Input*/
                if (input_path) free(input_path);
                input_path = malloc(strlen(optarg) + 1);
                strcpy(input_path, optarg);
                break;

            case 's':
                /*'s' as md5 Sum*/
                md5sum = true;
                break;

            case 'o':
                /*'o' as in Output*/
                if (output_path) free(output_path);
                output_path = malloc(strlen(optarg) + 1);
                strcpy(output_path, optarg);
                break;

            case 'v':
                /*'v' as in Verbose*/
                verbose = true;
                break;
        }
    }

    //verbose for getopt
    if (verbose) {
        printf("Getopt options :\n\tinput_path : %s\n", input_path);
        printf("\toutput_path : %s\n", output_path);
        printf("\tmd5sum : %d\n", md5sum);
        printf("\tVerbose : %d\n", verbose);
        printf("Start of scanning and saving :\n");
    }

    directory_t* dir = process_dir(input_path, md5sum, verbose);
    save_to_file(dir, output_path, input_path, verbose);
    free_dir(dir);
    free(output_path);

    if (verbose) {
        printf("End of scanning and saving.\n");
    }

    return 0;
}
