#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

int simple_num = 0;
int count = 1;

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
    printf("%d\n", getpid());

    signal(SIGINT, SigHndlr1);
    signal(SIGTERM, SigHndlr2);

    for (i = low; i < high; i++) {
        int flag = 0;
        for (j = 2; j < i; j++) {
            if (i % j == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            simple_num = i;
        }
    }

    printf("-1\n");
    return 0;
}