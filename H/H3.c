/*
- Let's say, that type of variable n is int and type of other variables is double
- Let's say, that firstly user should input an amount of numbers (n)
- Let's say, that if n is less than 1, then average equals 0
- Let's say, that input is correct (user inputs everything, that suits our types)
- Let's say, that if user inputs more numbers than n,
    then we count average only of the first n numbers
*/

#include <stdio.h>

double
count_average(int n)
{
    double sum = 0;
    double average;
    double item;
    int i;

    if (n < 1) {
        average = 0;
    } else {
        printf("%s", "Input numbers: ");
        for (i = 0; i < n; ++i) {
            scanf("%lf", &item);
            sum += item;
        }
        average = sum / n;
    }
    return average;
}

int
main(void)
{
    int n;
    double answer;

    printf("%s", "Hello! Input an amount of numbers: n = ");
    scanf("%d", &n);
    answer = count_average(n);
    printf("Average = %lf\n", answer);
    return 0;
}