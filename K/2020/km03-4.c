#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    unsigned int arr[64] = {0};
    for (int i = 1; i < argc; i = i + 2) {
        int tmp;
        sscanf(argv[i], "%o", &tmp);
        arr[tmp - 1] = 1;
    }
    unsigned long long res = 0;
    unsigned long long a = 1;
    for (int i = 0; i < 64; i++) {
        res = res + arr[i] * a;
        a = a * 2;
    }
    printf("xeu = %llu\n", res);
    printf("xeh = %llx\n", res);
    return 0;
}
