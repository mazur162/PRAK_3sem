#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv) {
    pid_t pid;
    int file;

    file = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC | O_RDONLY, 0600);

    int fd[2];
    pipe(fd);

    pid = fork();
    if (pid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[1], argv[1], NULL);
        exit(2);
    } else {
        dup2(fd[0], 0);
        dup2(file, 1);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[2], argv[2], NULL);
        exit(3);
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    return 0;
}
