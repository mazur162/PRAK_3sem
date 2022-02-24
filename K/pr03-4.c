#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

// ( cmd1 < file1 && cmd2 ) | cmd3 >> file2
// cmd1  пишет в fd[1], а cmd3 еще не читает. Возможно переполнение
// wait
// cmd2  cmd3


int main(int argc, char **argv) {
    int status;
    int in, out;
    int fd[2];
    pipe(fd);
    if (fork() == 0) { // son1  !!!
        close(fd[0]);
        if (fork() == 0) { // gson1
            in = open(argv[4], O_RDONLY);
            dup2(in, 0);
            close(in);
            dup2(fd[1], 1);
            close(fd[1]);
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }

        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
           if (fork()==0) { // gson2
               dup2(fd[1], 1);
               close(fd[1]);
               execlp(argv[2], argv[2], NULL);
               exit(127);
           }
        }
        close(fd[1]);  // проверять !!!
       while (wait(NULL) > 0) {;}
       exit(0);
    }
    close(fd[1]);// особое внимание
    if (fork()==0) {  // son2
        dup2(fd[0], 0);
        close(fd[0]);
        out = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0666);
        dup2(out, 1);
        close(out);
        execlp(argv[3], argv[3], NULL);
        exit(127);
    }
    close(fd[0]);
    while (wait(NULL) > 0) {;}
    return 0;
}