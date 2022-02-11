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

int main(int argc, char** argv)
{
    int fd12[2], fd24[2];
    int count, modulo;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &modulo);
    pipe(fd24);
    if (!fork())//P1
    {
        pipe(fd12);
        close(fd24[0]);
        if (!fork())//P2
        {
            close(fd12[1]);
            dup2(fd24[1],1);
            dup2(fd12[0],0);
            close(fd24[1]);
            close(fd12[0]);
            execlp(argv[3], argv[3], NULL);
            exit(1);
        }
        close(fd24[1]);
        close(fd12[0]);
    
        for(int i = 1; i <=count; i++ )
        {
            long long tmp1 = i*i;
            int tmp = (tmp1)%modulo;
            write(fd12[1], &tmp, sizeof(int));
        }
        close(fd12[1]);
        wait(NULL);
        exit(0);
    }
    if (!fork())//P3
    {
        close(fd24[1]);
        if (!fork())//P4
        {
            char c;
            while(read(fd24[0], &c, sizeof(char)))
            {
                if(c!=' ')
                     printf("%c", c);
                else
                     printf("%c", '\n');
                fflush(stdout);
            }
            close(fd24[0]);
            exit(0);
        }
        close(fd24[0]);
        wait(NULL);
        exit(0);
    }
    close(fd24[1]);
    close(fd24[0]);
    wait(NULL);
    wait(NULL);
    printf("%d\n",0);
    fflush(stdout);
    return 0;
}
