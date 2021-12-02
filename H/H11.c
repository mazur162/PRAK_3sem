// shell: cat /etc/passwd | grep user
// для запуска команд шелла я использую функции окружения PATH

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char** argv) {
    int fd[2];
    pipe(fd); // организован канал
    if(fork() == 0) { // сын
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp("cat", "cat", "/etc/passwd", NULL);
    }
    // родитель: 
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    execlp("grep", "grep", "user", NULL);
}