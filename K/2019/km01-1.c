#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
int main(int argc, char** argv)
{
    int mod;
    sscanf(argv[3], "%d", &mod);
    int f1 = open(argv[1], O_RDONLY);
    int f2 = open(argv[2], O_WRONLY | O_APPEND | O_CREAT, 0666);
    int x = 1;
    unsigned char b, c;
    while(read(f1, &c, 1)) {
        while(c > 0) {
            if (c & 1) {
                int s = 0;
                for(int i = 1; i <= x; i++)
                    s = s + i*i;
                s = s % mod;
                write(f2, &s, sizeof(int));
                printf("%d ", s);
            }
            c = c >> 1;
            x++;
        }
    }
    printf("\n");
    return 0;
}
