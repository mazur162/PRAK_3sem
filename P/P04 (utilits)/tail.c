#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>

/*
    tail filename
    вывод 10 последних строк файла возможны флаги:
    -n n последних строк
    +n с n-ой строки и до конца файла
*/

enum { BUF_SIZE = 208 };

int
open_file(int *fd, char *name)
{
    *fd = open(name, O_RDONLY);
    if (*fd == -1) {
        char *MSG = malloc(sizeof(*MSG) * (strlen(name) + 36));
        strcpy(MSG, "tail: ");
        strcat(MSG, name);
        strcat(MSG, ": No such file or directory\n");
        if (write(2, MSG, strlen(MSG)) != strlen(MSG)) {
            free(MSG);
            exit(1);
        }
        free(MSG);
        exit(1);
    }
    return 0;
}

int
print_tail(int *fd, int from_end, int num)
{
    char buf[BUF_SIZE];
    ssize_t n;
    if (from_end == 1) { // то есть с конца
        int lines = 1;
        while ((n = read(*fd, buf, BUF_SIZE)) > 0) {
            for (int i = 0; i < n; i++) {
                if (buf[i] == '\n')
                    lines++;
            }
        }
        num = lines + num;
        if (lseek(*fd, 0, SEEK_SET) == -1) {
            close(*fd);
            exit(1);
        }
    }
    int count = 1;
    int can_write = 0;
    while ((n = read(*fd, buf, BUF_SIZE)) > 0) {
        if (can_write == 0 && num > 1) for (int i = 0; i < n; i++) {
            if (buf[i] == '\n')
                count++;
            if (count == num) {
                can_write = 1;
                if (write(1, buf + i + 1, n - i - 1) != n - i - 1) {
                    close(*fd);
                    exit(1);
                }
                break;
            }
        } else if (write(1, buf, n) != n) {
            close(*fd);
            exit(1);
        }
    }
    close(*fd);
    return 0;
}

int
main(int argc, char **argv)
{
    if (argc == 1) {
        const char *MISSING_OP = "tail: missing file operand\n";
        if (write(2, MISSING_OP, strlen(MISSING_OP)) != strlen(MISSING_OP)) {
            exit(1);
        }
        exit(1);
    }

    if (argc > 3) {
        const char *TOO_MANY_ARGS = "cp: too many arguments\n";
        if (write(2, TOO_MANY_ARGS, strlen(TOO_MANY_ARGS)) != strlen(TOO_MANY_ARGS)) {
            exit(1);
        }
        exit(1);
    }

    int fd, res;
    if (argc == 2) {
        if ((res = open_file(&fd, argv[1])) != 0) {
            return res;
        }
    } else if ((res = open_file(&fd, argv[2])) != 0) {
        return res;
    }

    // DEFAULT: по умолчанию (если нет флагов), то мы выводим 10 последних строк
    int from_end = 1;
    int num = 10;

    if (argc == 3) {
        char *end;
        num = strtol(argv[1], &end, 10);

        if (num > 0) {
            from_end = 0;
        }

        if (num == INT_MAX) {
            const char *OUT_OF_DIAPASON = "tail: number of lines out of OUT_OF_DIAPASON\n";
            if (write(2, OUT_OF_DIAPASON, strlen(OUT_OF_DIAPASON)) != strlen(OUT_OF_DIAPASON)) {
                close(fd);
                exit(1);
            }
            close(fd);
            exit(1);
        }

        if (*end != '\0') {
            const char *WRONG_NUM = "tail: invalid number of lines\n";
            if (write(2, WRONG_NUM, strlen(WRONG_NUM)) != strlen(WRONG_NUM)) {
                close(fd);
                exit(1);
            }
            close(fd);
            exit(1);
        }
    }
    return print_tail(&fd, from_end, num);
}