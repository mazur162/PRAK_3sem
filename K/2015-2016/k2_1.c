#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// cmd1 | cmd2 | cmd3 >> file


int main(int argc, char **argv) {
    int file;
    int fd[2];
    int gd[2];
    pipe(fd);

    if (!fork()) {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[1], argv[1], NULL);
        exit(127);
    }
    close(fd[1]);

    pipe(gd);
    if (!fork()) {
        dup2(fd[0], 0);
        close(fd[0]);

        dup2(gd[1], 1);
        close(gd[1]);
        execlp(argv[2], argv[2], NULL);
        exit(127);
    }
    close(fd[0]);
    close(gd[1]);

    if (!fork()) {
        dup2(gd[0], 0);
        close(gd[0]);
        file = open(argv[4], O_WRONLY | O_APPEND | O_CREAT, 0600);
        dup2(file, 1);
        close(file);
        execlp(argv[3], argv[3], NULL);
        exit(127);
    }
    close(gd[0]);

    while (wait(NULL) > 0) {;}
    return 0;
}
