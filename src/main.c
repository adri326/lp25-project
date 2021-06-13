#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include <scan.h>
#include <save.h>

int main(int argc, char* argv[]) {
    printf("Greetings, fellow humans!\n");
    
    //getopt
    char output_path[200] = {0};
    char input_path[200] = {0};
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
        printf("Input : %s\n", input_path);
        printf("Output : %s\n", output_path);
        printf("md5sum : %d\n", md5sum);
        printf("Verbose : %d\n", verbose);
    }
    
    
    printf("start of test of scan and save on test folder\n");
    save_to_file(process_dir("test",  md5sum, verbose), "test/data/scan_save_test.txt", 0, "test", verbose);
    printf("end of test of scan and save on test folder\n");


    
    return 0;
}
