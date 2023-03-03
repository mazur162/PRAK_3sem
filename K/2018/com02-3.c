#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void
handler(int sig)
{
    if (sig == SIGUSR1)
    {
        count += 5;
    }
    else if (sig == SIGUSR2)
    {
        count -= 4;
    }

    printf("%d %d\n", sig, count);
    fflush(stdout);

    if (count < 0)
    {
        exit(0);
    }
}

int
main(void)
{
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    printf("%d\n", getpid());
    fflush(stdout);
	
    while (1)
    {
        usleep(5);
    }
	
    return 0;
}
