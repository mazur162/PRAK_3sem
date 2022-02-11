#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char ** argv)
{
    int fd32[2];
    int fd21[2];
    int fd13[2];
    pipe(fd32);
    pipe(fd21);
    pipe(fd13);
    long long count;
    scanf("%lld", &count);
    long long a = 1, b = 1;
    int pid1, pid2, pid3;

    if ((pid1 = fork()) == 0)
    {
        if ((pid3 = fork()) == 0)
        {
            close(fd21[1]);
            close(fd21[0]);
            close(fd13[1]);
            close(fd32[0]);
            while (1) {
                read(fd13[0], &a, sizeof(long long));
                read(fd13[0], &b, sizeof(long long));
  
                if (b > count) {
                    write(fd32[1], &a, sizeof(long long));
                    write(fd32[1], &b, sizeof(long long));
                    close(fd13[0]);
                    close(fd32[1]);
                    break;
                }
                printf("3 ");
                printf("%lld %lld\n", a, b);
                a = a + b;
                write(fd32[1], &b, sizeof(long long));
                write(fd32[1], &a, sizeof(long long));
            }
            exit(0);
        }
        close(fd32[0]);
        close(fd32[1]);
        close(fd21[1]);
        close(fd13[0]);
        while (1) {
            read(fd21[0], &a, sizeof(long long));
            read(fd21[0], &b, sizeof(long long));
            //printf("%lld %lld \n", a, b);
            if (b > count)
            {
                write(fd13[1], &a, sizeof(long long));
                write(fd13[1], &b, sizeof(long long));
                close(fd21[0]);
                close(fd13[1]);
                break;
            }
            printf("1 ");
            printf("%lld %lld\n", a, b);
            a = a + b;
            write(fd13[1], &b, sizeof(long long));
            write(fd13[1], &a, sizeof(long long)); 
        }
        wait(NULL);
        exit(0);

    }
    if ((pid2 = fork()) == 0) {
        close(fd13[0]);
        close(fd13[1]);
        close(fd32[1]);
        close(fd21[0]);
        while (1) {
            read(fd32[0], &a, sizeof(long long));
            read(fd32[0], &b, sizeof(long long));
            if (b > count) {
                write(fd21[1], &a, sizeof(long long));
                write(fd21[1], &b, sizeof(long long));
                close(fd32[0]);
                close(fd21[1]);
                break;
            }
            printf("2 ");
            printf("%lld %lld\n", a, b);
            a = a + b;
            write(fd21[1], &b, sizeof(long long));
            write(fd21[1], &a, sizeof(long long));
        }
        exit(0);
    }
    write(fd13[1], &a, sizeof(long long));
    write(fd13[1], &b, sizeof(long long));
    close(fd13[0]);
    close(fd13[1]);
    close(fd21[0]);
    close(fd21[1]);
    close(fd32[0]);
    close(fd32[1]);
    wait(NULL);
    wait(NULL);
    return(0);
}
