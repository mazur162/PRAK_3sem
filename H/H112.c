#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

/*
UPD: Добавлена обработка сигнала SIGTRAP
(Я просто невнимательно прочитала задание в прошлый раз :( )
*/

int count1 = 1;
void SigHndlr1 (int s) { 
    count1++;
    if (count1 == 3) {
        signal(SIGINT, SIG_DFL);
    }
} 

int count2 = 1;
void SigHndlr2 (int s) {
    if (count2 % 2 == 0) {
        int fd[2];
        pipe(fd);
        fork();
        fork();
    }
    count2++;
}

int
main(int argc, char** argv) {
    signal(SIGINT, SigHndlr1);
    signal(SIGTRAP, SigHndlr2);
    while (1);
    return 0;
}