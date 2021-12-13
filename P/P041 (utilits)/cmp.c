#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>

/*
    cmp filename1 filename2

   ОШИБКИ: 
    
    1: Отсутствует файл (или оба файла)
    2: Слишком большое количество аргументов для вызова cp
    3: Файл не существует
    4: Ошибка записи

    КОД ВОЗВРАТА ВСЕГДА 1 ПРИ ОШИБКЕ
    (как в оригинальном cmp)
*/

enum {BUF_SIZE = 208};

ssize_t
min(ssize_t a, ssize_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int
open_file(int *fd, char *filename) {
    *fd = open(filename, O_RDONLY);
    if (*fd == -1) {
        const char* NO_FILE = "cp: No such file or directory\n";
        if (write(2, NO_FILE, strlen(NO_FILE)) != strlen(NO_FILE)) {
            exit(1);
        }
        exit(1);
    }
    return 0;
}

int
num_symbols(unsigned long num){
    int count = 0;
    while (num == 0) {
        num = num / 10;
        count++;
    }
    return count;
}

int
different (int byte_num, int line_num, int *fd1, int *fd2, char *s1, char *s2) {
    char byte_num_str[byte_num / 10 + 1];
    gcvt(byte_num, byte_num / 10 + 1, byte_num_str);

    char line_num_str[line_num / 10 + 1];
    gcvt(line_num, line_num / 10 + 1, line_num_str);

    char *MSG = malloc(sizeof(*MSG) * (strlen(s1) + strlen(s2) + strlen(byte_num_str) + strlen(line_num_str) + 25));
    strcpy(MSG, s1);
    strcat(MSG, " ");
    strcat(MSG, s2);
    strcat(MSG, " differ: char ");
    strcat(MSG, byte_num_str);
    strcat(MSG, ", line ");
    strcat(MSG, line_num_str);
    strcat(MSG, "\n");

    if (write(2, MSG, strlen(MSG))  != strlen(MSG)) {
        exit(1);
    }
    free(MSG);

    close(*fd1);
    close(*fd2);
    return 0;
}

int
main(int argc, char **argv) {
    if (argc < 3) {
        const char *MISSING_OP = "cmp: missing file(s)\n";
        if (write(2, MISSING_OP, strlen(MISSING_OP)) != strlen(MISSING_OP)) {
            exit(1);
        }
        exit(1);
    }

    if (argc > 3) {
        const char *TOO_MANY_ARGS = "cmp: too many arguments\n";
        if (write(2, TOO_MANY_ARGS, strlen(TOO_MANY_ARGS)) != strlen(TOO_MANY_ARGS)) {
            exit(1);
        }
        exit(1);
    }
    int fd1, fd2, res;
    if ((res = open_file(&fd1, argv[1])) != 0) {
        return res;
    }
    if ((res = open_file(&fd2, argv[2])) != 0) {
        close(fd1);
        return res;
    }

    char buf1[BUF_SIZE], buf2[BUF_SIZE];
    unsigned long byte_num = 1, line_num = 1;
    ssize_t n1, n2;
    int flag = 0;
    while ((n1 = read(fd1, buf1, BUF_SIZE)) > 0 &&
            (n2 = read(fd2, buf2, BUF_SIZE)) > 0) {
        flag = 1;
        for (int i = 0; i < min(n1, n2); i++) {
            if ((char)buf1[i] != (char)buf2[i]) {
                return different(byte_num, line_num, &fd1, &fd2, argv[1], argv[2]);
            }
            byte_num++;
            if ((char)buf1[i] == '\n')
                line_num++;
        }
        if (n1 != n2) {
            return different(byte_num + 1, line_num, &fd1, &fd2, argv[1], argv[2]);
        }
    }
    
    if (((n1 == 0 && (n2 = read(fd2, buf2, BUF_SIZE)) > 0) ||
            (n2 == 0 && n1 > 0)) && (flag == 1)) {
        return different(byte_num, line_num, &fd1, &fd2, argv[1], argv[2]);
    }

    close(fd1);
    close(fd2);
    return 0;
}