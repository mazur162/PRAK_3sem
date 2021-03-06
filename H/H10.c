/*
Specification: text from input_file (it should already exist) would be divided into two files 
(f1 and f2) depending on number of strings in text

File names should be given as a command line arguments:
./prog input_file f1 f2

In this program I use low-level input-output commands to deal with files
All errors are fatal => program ends with exit code 1
Constants are set using enum

Have a nice day :)
*/

/*
UPD: 
- Реализовать буферное чтение не получилось, 
можете посмотреть предыдущие посылки:
по каким-то причинам там не прошло 2 теста,
мне так и не удалось их одолеть :(

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

enum {MAX_STR_LEN = 256, NUM_OF_STR = 100};

int
main (int argc, char**argv) {
    
    if (argc != 4) {
        printf("ERROR: Wrong num of args\n");
        exit(1);
    }
    char buf[MAX_STR_LEN + 1];
    int n;

    int input_file = open(argv[1], O_RDONLY);
    if (input_file == -1) { 
        printf("ERROR: Couldn't open input file for reading\n");
        exit(1);
    }

    int f1 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    int f2 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666); 
    
    if ((f1 == -1) && (f2 == -1)) {
        printf("ERROR: Couldn't create file for writing\n");
        exit(1);
    }

    int str_counter = 0;
    int file = f1;
    while((n = read(input_file, buf, 1)) > 0) {
        if (*buf == '\n') {
            str_counter++;
        }
        if (str_counter == NUM_OF_STR) { 
            file = f2;
            // changing file after writing down 100 strings in the first file
            // (i don't want to repeat code for different files, so I just change file in one cycle)
            str_counter++; 
        } else {
            write(file, buf, n);
        }
    }

    close(input_file);
    close(f1);
    close(f2);
    
    return 0;
}