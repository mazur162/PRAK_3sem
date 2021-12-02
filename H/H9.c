#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int
main (int argc, char **argv) {
    if (argc == 0) {
        printf("Error: Missing argument!\n");
    }
    
    if (strcmp(argv[1],"list") == 0) {
        execlp("gcc", "gcc", "1.c", "-o", "1.exe", "-lm", NULL);
    } else if (strcmp(argv[1],"vector") == 0) {
        char* arr[] = {"gcc", "1.c", "-o", "1.exe", "-lm", NULL};
        execvp("gcc", arr);
    } else {
        printf("Error: Wrong command line argument!\n");
    }
    return 0;
}