#include <stdio.h>

int 
main(void) {
    int a = 208;
    int a1 = 0xAB1; // число в шестнадцатиричной системе счисления
    float b = 2.5;
    double b1 = 4E-2; // экспоненциальная запись числа
    char c = '#';
    long g = 20*4; // вычисление значения, а затем преобразование типов
    long f = g+5L; // выражение использует ранее определённое значение
    printf("Task 6. Test 1. \n");    
    printf("integer a = %i\n", a);
    printf("integer a1 = %i\n", a1);
    printf("float b = %f\n", b);
    printf("double b1 = %lf\n", b1);
    printf("character c = %c\n", c);
    printf("long integer g = %li\n", g);
    printf("long integer f = %li\n", f);
    
    return 0;
}
