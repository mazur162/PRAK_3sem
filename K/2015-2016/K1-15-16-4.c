#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

int pid_son;
void f(int sig) {
    kill(pid_son, SIGINT);
    wait(NULL);
    printf("Process %d terminated\n", pid_son);
    exit(1);
}

//для тестировнаия
/*
void f1(int sig)
{
    kill(pid_son, sig);
}*/

int main(int argc, char** argv) {
    int N;
    sscanf(argv[1], "%d", &N);
    signal(SIGALRM, f);
    //signal(SIGQUIT, f1);//для тестирования - Ctrl-бэкслэш
    alarm(N);
    if(!(pid_son = fork()))
    {
        execvp(argv[2], argv+2);
        exit(1);
    }
    int s;
    wait(&s);
    signal(SIGALRM, SIG_IGN);
    if (WIFEXITED(s)) {
        printf("Process %d exited with code = %d\n", pid_son, WEXITSTATUS(s));
        return 0;
    }
    if(WIFSIGNALED(s)) {
        printf("Proccess %d signaled by sig = %d\n", pid_son, WTERMSIG(s));
        return 0;
    }
    return 0;
}