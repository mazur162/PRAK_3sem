#include <stdio.h>
#include <stdlib.h>

/*
Hello!
In this program I read numbers from text file and remember it in array.
As I understand, there is no prohibition for using array, because in task
description it was only said about prohibition for using additional files.

LIMITATIONS:
There should be only integers in text file, no other characters allowed, 
except whitespace characters, that separate numbers:
space (' '), tab ('\t'), newline ('\n') and so on

An amount of numbers is limited, otherwise it can cause segmentation fault
(size of array would be way too big)
*/

void
file_read_and_invert(FILE *file) {
    fseek(file, 0, SEEK_SET);
    int size = 0;
    while (!feof(file)) {
        int num;
        if (fscanf(file, "%d", &num) != 0) {
            size++;
        }
    }
    int array[size];

    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < size; i++) {
        fscanf(file, "%d", &array[i]);
    }

    for (int i = 0; i < size / 2; i++) {
        int t = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = t;
    }
    fclose(file);
    fclose(fopen("./file.txt", "wb")); // rewriting file

    file = fopen("./file.txt", "rb+");

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d", array[i]);
        if (i != size - 1) {
            fprintf(file, "%s", " "); // i don't want to put space in the end
        }
        fflush(file); // buffer resetting after writing
    }
}

void
file_output(FILE *file) {
    file = fopen("./file.txt", "rb");

    char c = fgetc(file);
    while (c != EOF)     {
        printf("%c", c);
        c = fgetc(file);
    }
    printf("\n");

    fclose(file);
}

int
main(void) {
    FILE *file = fopen("./file.txt", "rb+"); // mode: reading and writing
    if (file == 0) {
        printf("ERROR OPENING FILE");
    } else {
        printf("FILE: \n");
        file_output(file);
        printf("\n");

        file_read_and_invert(file);
        printf("MODIFIED FILE: \n");
        file_output(file);

        fclose(file); // file closing
    }
    return 0;
}
