#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
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

enum {BUF_SIZE = 1024};

ssize_t
min(ssize_t a, ssize_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

long
open_file(long *fd, char *filename) {
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

long
num_symbols(unsigned long num){
    long count = 0;
    while (num == 0) {
        num = num / 10;
        count++;
    }
    return count;
}

long
different (long byte_num, long line_num, long *fd1, long *fd2, char *s1, char *s2) {
    char byte_num_str[byte_num / 10 + 1];
    gcvt(byte_num, byte_num / 10 + 1, byte_num_str);

    char line_num_str[line_num / 10 + 1];
    gcvt(line_num, line_num / 10 + 1, line_num_str);

    char *MSG = malloc(sizeof(*MSG) * (strlen(s1) + strlen(s2) + strlen(byte_num_str) + strlen(line_num_str) + 23));
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
        free(MSG);
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
    long fd1, fd2, res;
    if ((res = open_file(&fd1, argv[1])) != 0) {
        return res;
    }
    if ((res = open_file(&fd2, argv[2])) != 0) {
        close(fd1);
        return res;
    }

    char buf1[BUF_SIZE], buf2[BUF_SIZE];
    long byte_num = 1, line_num = 1;
    long n1, n2;
    long flag = 0;
    do {
        n1 = read(fd1, buf1, BUF_SIZE);
        n2 = read(fd2, buf2, BUF_SIZE);

        flag = 1;
        if (errno != 0) {
            write(2, "cp: error while reading\n", 25);
            exit(1);
        }
        for (long i = 0; i < min(n1, n2); i++) {
            if ((buf1[i] != buf2[i]) && (buf1[i]!= -1) && (buf2[i] != -1)) {
                return different(byte_num, line_num, &fd1, &fd2, argv[1], argv[2]);
            }
            byte_num++;
            if (buf1[i] == '\n')
                line_num++;
        }
        if (n1 < n2) {
            char *EOF_MSG = malloc(sizeof(*EOF_MSG) * (strlen(argv[1]) + 13));
            strcpy(EOF_MSG, "cmp: EOF on ");
            strcat(EOF_MSG, argv[1]);
            strcat(EOF_MSG, "\n");
            if (write(2, EOF_MSG, strlen(EOF_MSG))  != strlen(EOF_MSG)) {
                free(EOF_MSG);
                exit(1);
            }
            free(EOF_MSG);
            return 0;
        }

        if (n1 > n2) {
            char *EOF_MSG = malloc(sizeof(*EOF_MSG) * (strlen(argv[2]) + 13));
            strcpy(EOF_MSG, "cmp: EOF on ");
            strcat(EOF_MSG, argv[2]);
            strcat(EOF_MSG, "\n");
            if (write(2, EOF_MSG, strlen(EOF_MSG)) != strlen(EOF_MSG)) {
                free(EOF_MSG);
                exit(1);
            }
            free(EOF_MSG);
            return 0;
        }
    } while ((n1 > 0) && (n2 > 0));
    
    if (((n1 == 0 && (n2 = read(fd2, buf2, BUF_SIZE)) > 0) ||
            (n2 == 0 && n1 > 0)) && (flag == 1)) {
        return different(byte_num, line_num, &fd1, &fd2, argv[1], argv[2]);
    }

    close(fd1);
    close(fd2);
    return 0;
}