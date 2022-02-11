#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    double x;
    sscanf(argv[1], "%lf", &x);
    int n;
    sscanf(argv[2], "%d", &n);
    double dx;
    sscanf(argv[3], "%lf", &dx);
    int fd12[2];
    int fd21[2];
    pipe(fd12);
    pipe(fd21);
    double sq_sum;
    double abs_sum;

    if (!fork()) {
        if (fork() == 0) {
            close(fd12[0]);
            close(fd21[1]);
            double tmp, tmp1;
            for (int i = 0; i < n; i++) {
                tmp1 = sin(x + i * dx);
                write(fd12[1], &tmp1, sizeof(double));
                read(fd21[0], &tmp, sizeof(double));
                sq_sum = sq_sum + (tmp * tmp);
            }
            printf("1: %.11g\n", sq_sum); //не знаю как написать мантиссу
            close(fd12[1]);
            close(fd21[0]);
            exit(0);
        }
        wait(NULL);
        close(fd12[0]);
        close(fd21[1]);
        close(fd12[1]);
        close(fd21[0]);
        exit(0);
    }

    if (!fork()) {
        if (fork() == 0) {
            close(fd12[1]);
            close(fd21[0]);
            double tmp, tmp1;
            for (int i = 0; i < n; i++) {
                tmp1 = cos(x + i * dx);
                write(fd21[1], &tmp1, sizeof(double));
                read(fd12[0], &tmp, sizeof(double));
                abs_sum = abs_sum + fabs(tmp);
            }
        printf("2: %.11g\n", abs_sum);
        close(fd12[0]);
        close(fd21[1]);
        exit(0);
        }
        close(fd12[0]);
        close(fd21[1]);
        close(fd12[1]);
        close(fd21[0]);
        wait(NULL);
        exit(0);
    }
    close(fd12[0]);
    close(fd21[1]);
    close(fd12[1]);
    close(fd21[0]);
    wait(NULL);
    wait(NULL);
    printf("00\n");
}