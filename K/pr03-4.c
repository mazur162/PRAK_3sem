#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

// ( cmd1 < file1 && cmd2 ) | cmd3 >> file2

int main(int argc, char **argv) {
    int status;
    int in, out;
    in = open(argv[4], O_RDONLY);
    out = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0666);
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        if (fork() == 0) {
            dup2(in, 0);
            close(fd[1]);
            close(fd[0]);
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }

        wait(&status);
        if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status)) {
            return 1;
        }

        if (fork()==0) {
            close(fd[1]);
            close(fd[0]);
            execlp(argv[2], argv[2], NULL);
            exit(127);
        }
    }
    if (fork()==0) {
        dup2(fd[0], 0);
        dup2(out, 1);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[3], argv[3], NULL);
        exit(127);
    }
    close(fd[1]);
    close(fd[0]);
    close(in);
    close(out);
	while (wait(NULL) > 0) {;}
    return 0;
}