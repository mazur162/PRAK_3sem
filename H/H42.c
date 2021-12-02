/*
- Let's say, that user inputs everything correctly
- Let's say, that all inputted numbers suits to int type
*/

#include <stdio.h>

int
U(int *X, int *Y) {
    int sum = 0;
    long int elem = 1;
    int i;
    for (i = 0; i < 15; i++) {
        elem = *(X + i);  // moving pointer and getting value
        elem *= *(Y + i);
        sum += elem; 
        elem = 1;
    }
    if (sum > 0) {
        sum = 0;
        for (i = 0; i < 20; i++) {
            elem = *(X + i); 
            elem *= *(X + i);
            sum += elem;
            elem = 1;
        }
    } else {
        sum = 0;
        for (i = 9; i < 20; i++) {
            elem = *(Y + i);
            elem *= *(Y + i);
            sum += elem;
            elem = 1;
        }
    }
    return sum;
}

int
main(void) {
    int i;
    int X[20];
    int Y[20];
    printf("Hello! Please input array values separated by space or new line:\n");
    printf("In each pair of numbers first comes X[i], then Y[i]\n");
    for (i = 0; i < 20; i++) {
        scanf("%d %d", X + i, Y + i);
    }
    printf("%d\n", U(X,Y));
}