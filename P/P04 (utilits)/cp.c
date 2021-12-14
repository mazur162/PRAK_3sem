#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
/*
    cp old_file new_file

    ОШИБКИ: 
    
    1: Отсутствует файл (или оба файла)
    2: Слишком большое количество аргументов для вызова cp
    3: old_file не существует
    4: Ошибка записи
    5: Ошибка при вызове stat (права доступа)
    6: Ошибка при открытии файла
    7: Ошибка чтения
    8: Файлы идентичны (один и тот же файл)

    КОД ВОЗВРАТА ВСЕГДА 1 ПРИ ОШИБКЕ
    (как в оригинальном cp)
*/

enum {BUF_SIZE = 208};

int
main(int argc, char **argv)
{
    if (argc < 3) {
        const char *MISSING_OP = "cp: missing file(s)\n";
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

    int old_file_fd = open(argv[1], O_RDONLY);
    if (old_file_fd == -1) {
        char* NO_FILE = malloc(sizeof(*NO_FILE) * (strlen(argv[1]) + 30));
        strcpy(NO_FILE, "cp: ");
        strcat(NO_FILE, argv[1]);
        strcat(NO_FILE, ": No such file or directory\n");
        if (write(2, NO_FILE, strlen(NO_FILE)) != strlen(NO_FILE)) {
            free(NO_FILE);
            exit(1);
        }
        free(NO_FILE);
        exit(1);
    }

    struct stat file1_s;
    if (stat(argv[1], &file1_s) == -1) {
        close(old_file_fd);
        exit(1);
    }

    int new_file_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, file1_s.st_mode);
    if (new_file_fd == -1) {
        close(old_file_fd);
        exit(1);
    }

    struct stat file2_s;
    if (stat(argv[2], &file2_s) == -1) {
        close(old_file_fd);
        exit(1);
    }

    if (file1_s.st_ino == file2_s.st_ino) {
        char* IDENT_FILES = malloc(sizeof(*IDENT_FILES) * (strlen(argv[1]) + strlen(argv[2]) + 39));
        strcpy(IDENT_FILES, "cp: ");
        strcat(IDENT_FILES, argv[1]);
        strcat(IDENT_FILES, " and ");
        strcat(IDENT_FILES, argv[2]);
        strcat(IDENT_FILES, " are identical (not copied).\n");
        if (write(2, IDENT_FILES, strlen(IDENT_FILES))  != strlen(IDENT_FILES)) {
            free(IDENT_FILES);
            exit(1);
        }
        free(IDENT_FILES);
        exit(1);
    }

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(old_file_fd, buf, BUF_SIZE)) > 0) {
        if (write (new_file_fd, buf, n) != n) {
            close(old_file_fd);
            close(new_file_fd);
            exit(1);
        }
    }
    if (errno != 0) {
        const char *READ_ERR = "cp: error while reading\n";
        if (write(2, READ_ERR, strlen(READ_ERR)) != strlen(READ_ERR)) {
            exit(1);
        }
        exit(1);
    }

    close(old_file_fd);
    close(new_file_fd);
    return 0;
}