#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

volatile int simple_num = 0;
volatile int count = 1;

void
SigHndlr1(int s) {
    if (count == 4) {
        _exit(0);
    }
    printf("%d\n", simple_num);
    count++;
    signal(SIGINT, SigHndlr1); // возобновляю обработчик сигнала
}

void
SigHndlr2(int s) {
    _exit(0);
}

int
main(void) {
    long long i, j;
    long long low, high;
    scanf("%lld%lld", &low, &high);
    if (high < 0) {
        printf("-1\n");
        return 0;
    }

    signal(SIGINT, SigHndlr1);
    signal(SIGTERM, SigHndlr2);

    int cnt = 0;

    if (low < 2) {
        low = 2;
    }
    if ((low == 2) && (high >= 2)) {
        simple_num = 2;
        printf("%d\n", getpid());
        cnt = 1;
        low = 3;
    }
    if ((low == 3) && (high >= 3)) {
        simple_num = 3;
        if (cnt==0) {
          printf("%d\n", getpid());
          cnt = 1;
        }
        low = 5;
    }

    long long simple_nums[10000] = {3};
    int p = 1;

    int lim_int = sqrt(high) + 1; // макс. число на которое имеет смысл делить
    for (i = 5; i < high; i += 2) {  //INT_MAX
        int flag = 0;
        int k = sqrt(llabs(i)) + 1;
        j = 0;
        while (j < p && simple_nums[j] <= k) {
            if (i % simple_nums[j] == 0) {
                flag = 1;
                break;
            }
            j++;
        }
        if (flag == 0) {
            if (i>=low) {  
                simple_num = i;
                if (cnt==0) {
                   printf("%d\n", getpid());
                   cnt = 1;
                }
            }

        }
            if (i <= lim_int) {
   		   //if (p > sizeof(simple_nums)/sizeof(p[0])) {
               // чтобы не переполнять массив
                simple_nums[p] = simple_num;
                p++;
               //}
            } else {
                if (i < low) {
                     low = i;
                }
            }
    }
    printf("-1\n");
    return 0;
}
