#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

int 
main(void) {
    int fd;
    if ((fd = open("input.txt", O_RDONLY)) == -1) {
        exit(1);
    }
    char filename1[10] = "tmpXXXXXX";
    char filename2[10] = "tmpXXXXXX";
    int tmp1 = mkstemp(filename1);
    int tmp2 = mkstemp(filename2);

    int num;

    while (read(fd, &num, sizeof(num)) > 0) {
        if (num < 0) {
            write(tmp1, &num, sizeof(num));
        } else {
            write(tmp2, &num, sizeof(num));
        }
    }
    close(fd);
    if ((fd = open("input.txt", O_APPEND | O_TRUNC | O_RDWR)) == -1) {
        exit(1);
    }

    lseek(fd, 0, SEEK_SET);
    lseek(tmp1, 0, SEEK_SET);
    while (read(tmp1, &num, sizeof(num)) > 0) {
        write(fd, &num, sizeof(num));
    }
    lseek(tmp2, 0, SEEK_SET);
    while (read(tmp2, &num, sizeof(num)) > 0) {
        write(fd, &num, sizeof(num));
    }

    close(tmp1);
    close(tmp2);
    unlink(filename1);
    unlink(filename2);
    close(fd);
    return 0;
}
