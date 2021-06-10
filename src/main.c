#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char* argv[]) {
    printf("Greetings, fellow humans!\n");
    
    //getopt
    char output_path[200] = {0};
    char input_path[200] = {0};
    bool md5sum = false;
    int opt = 0;
    while((opt = getopt(argc, argv, "i:so:")) != -1) {
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
        }
    }
    




    
    return 0;
}
