#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd[2] = {0};
    pipe(fd);
    int pid_p1 = fork();
    if (pid_p1 == 0) {
        int pid_p3 = fork();
        if (pid_p3 == 0) {
            close(fd[1]);
            int len = 0;
            char c, first, prev;
            while (read(fd[0], &c, 1) > 0) {
                if (c == '\n') {
                    write(1, &first, 1);
                    if (len > 0) {
                        if (len > 1) {
                            write(1, &prev, 1);
                        }
                        printf("\n");
                    }
                    len = 0;
                } else {
                    if (len == 0) {
                        first = c;
                    }
                    len++;
                }
                prev = c;
            }
        }
        close(fd[0]);
        close(fd[1]);
        waitpid(pid_p3, NULL, 0);
        exit(0);
    }

    int pid_p2 = fork();
    if (pid_p2 == 0) {
        close(fd[0]);
        int pid_p4 = fork();
        if (pid_p4 == 0) {
            int input = open(argv[1], O_RDONLY);
            if (input == -1) {
                exit(1);
            }
            fseek(input, 0, SEEK_END);
            int size = ftell(input);
            fseek(input, 0, SEEK_SET);
            if ((char*)malloc((size + 1) * sizeof(char)) == NULL) {
                exit(1);
            }
            char *str = (char*)malloc((size + 1) * sizeof(char)); 
            while (fgets(str, size + 1, input) != 0) {
                write(fd[1], str, strlen(str));
                fgets(str, size + 1, input);
            }
            free(str);
        }
        close(fd[0]);
        close(fd[1]);
        waitpid(pid_p4, NULL, 0);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid_p1, NULL, 0);
    waitpid(pid_p2, NULL, 0);
    return 0;
}