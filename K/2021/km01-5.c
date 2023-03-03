#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <string.h>

volatile long long simple_num = 0;
volatile int count = 1;

void
SigHndlr1(int s) {
    if (count == 4) {
        _exit(0);
    }
    printf("%lld\n", simple_num);
    count++;
    signal(SIGINT, SigHndlr1); // возобновляю обработчик сигнала
}

void
SigHndlr2(int s) {
    _exit(0);
}

int
main(void) 
{
    long long i, j, low, high, k;
    int flag;
    scanf("%lld%lld", &low, &high);

    signal(SIGINT, SigHndlr1);
    signal(SIGTERM, SigHndlr2);

    printf("%d\n", getpid());

    if (low < 2) 
    {
        low = 2;
    }

    if (low >= high) 
    {
        printf("-1\n");
        return 0;
    }

    char *primes = (char *) malloc(sizeof(char)*high); // char primes[high];
    if (primes == NULL) 
    {
        printf("QKRQ\n");
        return -1;
    }
    
    memset(primes, 0, high);
    for (i = 0; i < high; i++) 
    {
        if (primes[i])
           printf("QQ\n");
    }

    for (i = 2; i < high; i++) 
    {
        if (!primes[i])
        {
            if (i >= low) 
            {
                simple_num = i;
                printf("%lld\n", simple_num);
            }
            for (j = i; j < high; j += i) 
            {
                primes[j] = -1;
            }
        }
    }
    free(primes);
    
    // for (i = low; i < high; i++) {
    //     flag = 0;
    //     k = sqrt(abs(i)) + 1;
    //     for (j = 2; (j < k) && !flag; j += 1) {
    //         flag = !(i % j);
    //     }
    //     if (!flag) {
    //         simple_num = i;
    //     }
    // }

    printf("-1\n");
    return 0;
}