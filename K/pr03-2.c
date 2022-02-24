#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int N;
    scanf("%d", &N);
    int i;
    for (i = 1; i <= N; i++) {
        printf("%d", i);
        fflush(stdout);
        if (i < N) {
            printf(" ");
            fflush(stdout);
            if (fork()) {
                break;
            }
        } else {
            break;
        }
    }
    wait(NULL);
    if (i == 1) {
        printf("\n");
    }
}
