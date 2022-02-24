#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int fd = creat(argv[1], 0777);
    for (int i = 1; i < 10; i++) {
        double a = (double) i;
        write(fd, &a, sizeof(a));
    }
    close(fd);
    fd = open(argv[1], O_RDONLY);
    double buff;
    while (read(fd, &buff, sizeof(buff))) {
        printf("in = %lf\n", buff);
    }
    close(fd);
    fd = open(argv[1], O_RDWR);
    int N;
    double inp, out, pout;
    sscanf(argv[2], "%d", &N);
    if (N < 1) {
        exit(0);
    }
    read(fd, &pout, sizeof(pout));
    for (int i = 1; i < N; i++) {
        read(fd, &inp, sizeof(inp));
        out = 1.5 * inp + 0.5 * pout;
        lseek(fd, i * sizeof(double), 0);
        write(fd, &out, sizeof(out));
        pout = out;
    }
    close(fd);
    fd = open(argv[1], O_RDONLY);
    while (read(fd, &buff, sizeof(buff))) {
        printf("out = %lf\n", buff);
    }
    close(fd);
    return 0;
}