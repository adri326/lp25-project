#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include <tree.h>
#include <scan.h>
#include <save.h>

int main(int argc, char* argv[]) {
    //getopt
    char output_path[200] = "result_of_scan.txt";
    char input_path[200] = "./";
    bool md5sum = false;
    bool verbose = false;
    int opt = 0;
    while((opt = getopt(argc, argv, "i:so:v")) != -1) {
        switch (opt) {
            case 'i':
                /*'i' as in Input*/
                strcpy(input_path, optarg);
                break;

            case 's':
                /*'s' as md5 Sum*/
                md5sum = true;
                break;

            case 'o':
                /*'o' as in Output*/
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

    if (verbose) {
        printf("End of scanning and saving.\n");
    }

    return 0;
}
