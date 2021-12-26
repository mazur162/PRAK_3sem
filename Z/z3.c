#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int flag_execute = 0, flag_end = 0;

void
signal_handler(int sig)
{
    if (sig == SIGUSR1) {
        static int counter = 0;
        counter++;
        if (counter == 3) {
            flag_execute++;
            counter = 0;
        }
    }
    if (sig == SIGTERM) {
        flag_end = 1;
    }
}

int
main(int argc, char **argv)
{
    int p2;
    for (p2 = 1; p2 < argc; p2++) {
        if (strcmp(argv[p2], "--") == 0) {
            break;
        }
    }
    argv[p2] = NULL;
    p2++;
    int fd[2];
    if (pipe(fd) < 0) {
        return 1;
    }
    signal(SIGUSR1, signal_handler);
    signal(SIGTERM, signal_handler);
    pid_t pid;
    if ((pid = fork()) == 0) {
        while (1) {
            while (!flag_execute && !flag_end) {
                usleep(100);
            }
            if (flag_end && !flag_execute) {
                break;
            }
            if (fork() == 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                execvp(argv[1], argv + 1);
                exit(127);
            }
            if (fork() == 0) {
                dup2(fd[0], 0);
                close(fd[0]);
                close(fd[1]);
                execvp(argv[p2], argv + p2);
                exit(127);
            }
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
            flag_execute--;
        }
        exit(0);
    }
    signal(SIGUSR1, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    int num;
    while (scanf("%d", &num) == 1) {
        if (num == 0) {
            kill(pid, SIGUSR1);
        } else {
            usleep(num);
        }
    }
    kill(pid, SIGTERM);
    wait(NULL);
}