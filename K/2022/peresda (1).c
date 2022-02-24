#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd[2] = {0};
    if (pipe(fd) == -1) {
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == 0) {
        exit(1);
    }
    fseek(input, 0, SEEK_END);
    int size = ftell(input);
    fseek(input, 0, SEEK_SET);
    FILE *read_fd = fdopen(fd[0], "r");
    FILE *write_fd = fdopen(fd[1], "w");
    int pid_p1 = fork();
    if (pid_p1 == 0) {
        fclose(write_fd);
        int pid_p3 = fork();
        if (pid_p3 == 0) {
            char *str1 = calloc(size + 1, sizeof(*str1));
            while (fscanf(read_fd, "%s", str1) == 1) {
                if (str1[0] != '\n') {
                    printf("%c", str1[0]);
                    if (str1[1] != '\n') {
                        printf("%c", str1[1]);
                    } else {
                        printf("%c", str1[0]);
                    }
                }
                for (int i = 0; i < size + 1; ++i) {
                    str1[i] = 0;
                }
            }
            printf("\n");
            fclose(read_fd);
            fclose(input);
            free(str1);
            exit(0);
        }
        fclose(input);
        fclose(read_fd);
        waitpid(pid_p3, NULL, 0);
        exit(0);
    }
    int pid_p2 = fork();
    if (pid_p2 == 0) {
        fclose(read_fd);
        int pid_p4 = fork();
        if (pid_p4 == 0) {
            char *str2 = calloc(size + 1, sizeof(*str2));
            if (str2 == 0) {
                fclose(write_fd);
                fclose(input);
                exit(1);
            }
            while (fgets(str2, size + 1, input)) {
                fprintf(write_fd, "%s", str2);
                fflush(write_fd);
                fgets(str2, size + 1, input);
                for (int i = 0; i < size + 1; ++i) {
                    str2[i] = 0;
                }
            }
            free(str2);
            fclose(input);
            fclose(write_fd);
            exit(0);
        }
        fclose(write_fd);
        fclose(input);
        waitpid(pid_p4, NULL, 0);
        exit(0);
    }
    fclose(input);
    waitpid(pid_p1, NULL, 0);
    waitpid(pid_p2, NULL, 0);
    return 0;
}