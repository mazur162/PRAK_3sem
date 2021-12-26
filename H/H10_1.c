/*
Specification: user should input text in stdin, after that this would be 
written down into input_file, then text would be divided into two files 
(f1 and f2) depending on nmber of strings in text

File names should be given as a command line arguments:
./prog input_file f1 f2

In this program I use low-level input-output commands to deal with files
All errors are fatal => program ends with exit code 1
Constants are set using enum

Have a nice day :)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

enum {MAX_STR_LEN = 1024, NUM_OF_STR = 10};

int
main (int argc, char**argv) {
    
    if (argc != 4) {
        printf("ERROR: Couldn't create file\n");
        exit(1);
    }
    char buf[MAX_STR_LEN];
    int n;

    int input_file = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY);
    if (input_file == -1) {
        printf("ERROR: Couldn't create input file\n");
        exit(1);
    }

    while ((n = read(0, buf, MAX_STR_LEN)) > 0) {   // fd == 0 - stdin
        write(input_file, buf, n);
    }
    close(input_file);

    input_file = open(argv[1], O_RDONLY);
    struct stat s; 
    if (input_file == -1) { 
        printf("ERROR: Couldn't open input file for reading\n");
        exit(1);
    }

    stat(argv[1], &s);
    int f1 = creat(argv[2], s.st_mode);
    int f2 = creat(argv[3], s.st_mode); 
    
    if ((f1 == -1) && (f2 == -1)) {
        printf("ERROR: Couldn't create files for writing\n");
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

    close(f1);
    close(f2);
    
    return 0;
}