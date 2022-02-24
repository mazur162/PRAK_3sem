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

int speed;
int dist = 0;

void SigUser1 (int sig)
{
    if (sig = SIGUSR1)
    {
        speed++;
    }
}

void SigUser2 (int sig)
{
    if (sig = SIGUSR2)
    {
        speed--;
    }
}

void SigTerm (int sig)
{
    if (sig = SIGTERM)
    {
        printf("%d\n", dist);
        exit(0);
    }
}

void SigAlrm (int sig)
{
    if (sig = SIGALRM)
    {
        dist += speed;
    }
}

int main (int argc, char **argv)
{
    signal(SIGUSR1, SigUser1);
    signal(SIGUSR2, SigUser2);
    signal(SIGTERM, SigTerm);
    signal(SIGALRM, SigAlrm);

    printf("%d\n", getpid());

    sscanf(argv[1], "%d", &speed);

    while (1)
    {
        alarm(1);
        sleep(1);
    }
}