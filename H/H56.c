/*
1)  to compile with function definition:
        gcc -DFUNC=\"func_name\" -o prog prog.c -lm

    to run program: 
        ./prog a b h

    example:
        gcc -DFUNC=\"fabs\" -o prog prog.c -lm
        ./prog -5 5 1
    result:
    
            TABLE OF FUNCION VALUES
        |--------------------------|
        |    x      |   fabs(x)    |
        |--------------------------|
        | -5.000000 |     5.000000 |
        |--------------------------|
        | -4.000000 |     4.000000 |
        |--------------------------|
        | -3.000000 |     3.000000 |
        |--------------------------|
        | -2.000000 |     2.000000 |
        |--------------------------|
        | -1.000000 |     1.000000 |
        |--------------------------|
        |  0.000000 |     0.000000 |
        |--------------------------|
        |  1.000000 |     1.000000 |
        |--------------------------|
        |  2.000000 |     2.000000 |
        |--------------------------|
        |  3.000000 |     3.000000 |
        |--------------------------|
        |  4.000000 |     4.000000 |
        |--------------------------|
        |  5.000000 |     5.000000 |
        |--------------------------|


2)  to compile without function definition:
        gcc -o prog prog.c -lm

    to run program: 
        ./prog func_name a b h

    example:
        gcc -o prog prog.c -lm
        ./prog sin -10 0 2
    result:

           TABLE OF FUNCION VALUES
        |--------------------------|
        |    x      |     sin(x)   |
        |--------------------------|
        |-10.000000 |     0.544021 |
        |--------------------------|
        | -8.000000 |    -0.989358 |
        |--------------------------|
        | -6.000000 |     0.279415 |
        |--------------------------|
        | -4.000000 |     0.756802 |
        |--------------------------|
        | -2.000000 |    -0.909297 |
        |--------------------------|
        |  0.000000 |     0.000000 |
        |--------------------------|


if user has inputted wrong number of arguments or wrong definition of function,
that would be a fatal error and program would exit with code 1
*/


#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <math.h> 

typedef double (*tf)(double);

const char *str[7] = {"sin","cos","tan","log", "fabs", "sqrt", "exp"};
tf m[7] = {sin, cos, tan, log, fabs, sqrt, exp};

double x;
double a, b, h;
int i;

void
table_output(int argc, char** argv) {
    for (i = 0; i < 7; i++) {
        if (!strcmp(str[i], argv[1])) {
            break; 
        }
    }
        
    if (i == 7) {
        printf("ERROR: Wrong function name\n");
        exit(1);
    }

    a = atof(argv[argc - 3]);
    b = atof(argv[argc - 2]);
    h = atof(argv[argc - 1]);


    printf("\n   TABLE OF FUNCION VALUES\n");
    printf("|--------------------------|\n");
    printf("|    x      |    %*s%s", 4, str[i], "(x)   |\n");
    printf("|--------------------------|\n");

    x = a;
    while (x <= b) {
        printf("|%*f | %*f |\n", 10, x, 12, m[i](x));
        printf("|--------------------------|\n");
        x += h;
    }
}

int
main(int argc, char **argv) {

    #ifndef FUNC
        if (argc != 5) {
            printf("ERROR: Wrong number of command line arguments\n");
            exit(1);
        }

    table_output(argc, argv);
        
    #else
        if (argc != 4) {
            printf("ERROR: Wrong number of command line arguments\n");
            exit(1);
        }

    table_output(argc, argv);
    
    #endif
        return 0;
}