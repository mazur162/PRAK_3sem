#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int proc (void)
{
    int pid = fork();
    if (!pid)
    {
        write(1, "1\n", 2);
    }
    wait(NULL);
    return pid;
}

int main (void)
{
    int k = proc() + proc() +  proc();

    return 0;
