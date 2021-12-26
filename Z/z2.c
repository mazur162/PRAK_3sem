#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>

char ***arg = NULL;
int argc = 0;

void
delete(void)
{
    for (int i = 0; i < argc; i++) {
        int j = 0;
        while (arg[i][j] != NULL) {
            free(arg[i][j]);
            j++;
        }
        free(arg[i]);
    }
    free(arg);
}

int
main(void)
{
    int c;
    while ((c = getchar())!= EOF) {
        argc++;
        arg = realloc(arg, argc * sizeof(char **));
        arg[argc - 1] = NULL;
        while (c != EOF && isspace(c)) {
            c = getchar();
        }
        int size = 0, num_arg = 0;
        while (c != EOF && c != ';' && c != '\n') {
            if (num_arg == size) {
                if (size == 0) {
                    size = 1;
                } else {
                    size = size * 2;
                }
                arg[argc - 1] = realloc(arg[argc - 1], size * sizeof (char *));
            }
            arg[argc - 1][num_arg] = NULL;
            int size_word = 0, num_sym_in_word = 0;
            while (c != EOF && !isspace(c) && c != ';') {
                if (num_sym_in_word == size_word) {
                    size_word = (size_word == 0) ? (1) : (size_word * 2);
                    arg[argc - 1][num_arg] = realloc(arg[argc - 1][num_arg], size_word * sizeof(char));
                }
                arg[argc - 1][num_arg][num_sym_in_word] = c;
                ++num_sym_in_word;
                c = getchar();
            }
            if (num_sym_in_word == size_word) {
                size_word = (size_word == 0) ? (1) : (size_word * 2);
                arg[argc - 1][num_arg] = realloc(arg[argc - 1][num_arg], size_word * sizeof(char));
            }
            arg[argc - 1][num_arg][num_sym_in_word] = '\0';
            ++num_arg;
            while (c != EOF && isspace(c) && c != '\n') {
                c = getchar();
            }
        }
        if (num_arg == size) {
            if (size == 0) {
                    size = 1;
               } else {
                    size = size * 2;
               }
            arg[argc - 1] = realloc(arg[argc - 1], size * sizeof(char *));
        }
        arg[argc - 1][num_arg] = NULL;
    }
    atexit(delete); // указанная функция будет вызвана при exit
    int status;
    pid_t pid;
    for (int i = 0; i < argc; i++) {
        if ((pid = fork()) == 0) {
            execvp(arg[i][0], arg[i]);
            exit(127);
        } else if (pid == -1) {
            exit(1);
        }
        wait(&status);
    }
    if (WIFEXITED(status)) {
        exit(WIFEXITED(status));
    }
    if (WIFSIGNALED(status)) {
        exit(128 + WTERMSIG(status));
    }
}