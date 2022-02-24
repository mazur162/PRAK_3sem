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
    int i, j;
    int low, high;
    scanf("%d%d", &low, &high);
    if (high < 0) {
        return 0;
    }

    signal(SIGINT, SigHndlr1);
    signal(SIGTERM, SigHndlr2);

    printf("%d\n", getpid());

    if (low < 2) {
        low = 2;
    }
    if ((low == 2) && (high >= 2)) {
        simple_num = 2;
        low = 3;
    }

    for (i = low; i < high; i++) {
        int flag = 0;
        int k = sqrt(abs(i)) + 1;
        for (j = 3; j < k; j += 2) {
            if (i % j == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            printf("%d\n", simple_num);
            simple_num = i;
        }
    }

    printf("-1\n");
    return 0;
}
