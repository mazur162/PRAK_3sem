#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count(long long value) {
    int c = 0;
    while (value > 0) {
        c += value % 2;
        value /= 2;
    }
    return c;
}

int main(void) {
    long long value;
    scanf("%lld", &value);
    printf("%d\n", count(value));
    return 0;
}