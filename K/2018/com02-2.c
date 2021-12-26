#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>


int main(int argc, char **argv) {
    pid_t p1, p2, p3, p4, p5;
    int fd42[2], fd25[2];
    pipe(fd42);
    pipe(fd25);
    
    if ((p1 = fork()) == 0) {
        if ((p4 = fork()) == 0) {
            dup2(fd42[1], 1);
            close(fd42[0]);
            close(fd42[1]);
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }
        close(fd42[0]);
        close(fd42[0]);
        wait(NULL);
        return 0;
    }
    
    
    if ((p2 = fork()) == 0) {
        close(fd42[1]);
        close(fd25[0]);
        int len = 0;
        char c, buf[34];
        while (read(fd42[0], &c, 1) > 0) {
            if (isspace(c)) {
                buf[len] = '\n';
                if (len > 0 && buf[len - 1] % 2 == 0) {
                    write(fd25[1], buf, len + 1);
                }
                len = 0;
            }
            if (c >= '0' && c <= '9') {
                buf[len] = c;
                len++;
            }
        }
        buf[len] = '\n';
        if (len > 0 && buf[len - 1] % 2 == 0) {
            write(fd25[1], buf, len + 1);
        }
        close(fd42[0]);
        close(fd25[1]);
        return 0;
    }
    
    if ((p3 = fork()) == 0) {
        if ((p5 = fork()) == 0) {
            dup2(fd25[0], 0);
            close(fd25[0]);
            close(fd25[1]);
            execlp(argv[2], argv[2], NULL);
            exit (127);
        }
        close(fd25[0]);
        close(fd25[1]);
        wait(NULL);
        return 0;
    }
    
    close(fd25[0]);
    close(fd25[1]);
    close(fd42[0]);
    close(fd42[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    return 0;
}
