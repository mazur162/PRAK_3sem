#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int
main(void) {
    int N;
    scanf("%d", &N);
    int i, j;
    j = 1;
    for (i = j; i < N + 1; i++) {
        j++;
        pid_t pid = fork();

        if (pid != 0) {
            printf("%d", i);

            if (i == N) {
                printf("\n");
                return 0;
            } else {
                printf(" ");
                wait(NULL);
            }

            break;
            wait(NULL);
            return 0;
        }
    }
}