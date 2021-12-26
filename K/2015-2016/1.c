#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <ctype.h>
#include <math.h>

int
main(void) {
    int i, j;
    int flag = 0;
    char a[8][8], b[8][8];
    char filename[10] = "tmpXXXXXX";
    int tmp = mkstemp(filename);
    while (1) {
        for (i = 0; i < 8; i++) {
            char c0, c1;
            while (isspace(c1 = getchar())) {;}
            c0 = getchar();
            if (c0 == EOF && c1 == EOF) {
                flag = 1;
                break;
            }

            if (c0 >= 'A') {
                c0 = c0 - 'A' + 10;
            } else {
                c0 = c0 - '0';
            }

            if (c1 >= 'A') {
                c1 = c1 - 'A' + 10;
            } else {
                c1 = c1 - '0';
            }

            j = 0;
            while (j < 4) {
                a[i][7 - j] = c0 % 2;
                c0 = c0 / 2;
                j++;
            }
            while (j < 8) {
                a[i][7 - j] = c1 % 2;
                c1 = c1 / 2;
                j++;
            }
        }
        if (flag == 1) {
            break;
        }
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                b[i][j] = a[j][i];
            }
        }
        for (i = 0; i < 8; i++) {
            int num_b = 0, k = 0;
            for (j = 7; j >= 0; j--) {
                num_b += b[i][j] * pow(2, k);
                k++;
            }
            lseek(tmp, 0, SEEK_END);
            write(tmp, &num_b, sizeof(num_b));
        }
    }

    int n;
    lseek(tmp, 0, SEEK_SET);
    while (read(tmp, &n, sizeof(n)) > 0) {
        printf("%d\n", n);
    }
    unlink(filename);

    return 0;
}