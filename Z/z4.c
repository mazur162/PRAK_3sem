#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <ctype.h>

int
main (void) {
    int i;
    unsigned int N, K;
    scanf("%u %u", &N, &K);
    char filename[10] = "tmpXXXXXX";
    int tmp_fd = mkstemp(filename);
    write(tmp_fd, &K, sizeof(K));

    union semun {
        int     val;            /* value for SETVAL */
        struct  semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
        u_short *array;         /* array for GETALL & SETALL */
     };

    int semid = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);
    u_short *arr = malloc(N*sizeof(u_short));
    arr[0] = 1;
    for (i = 1; i < N; i++) {
        arr[i] = 0;
    }
    semctl(semid, 0, SETALL, (union semun) {.array = arr});
    free(arr);

    for (i = 0; i < N; i++) {
        if (fork() == 0) {
            while (1) {
                unsigned int num;
                semop(semid, &(struct sembuf) {i, -1, 1}, 1);
                lseek(tmp_fd, -sizeof(unsigned int), SEEK_END);
                read(tmp_fd, &num, sizeof(num));
                if (num == 0) {
                    semop(semid, &(struct sembuf) {(i + 1) % N, +1, 0 }, 1);
                    return 0;
                }
                num--;
                write(tmp_fd, &num, sizeof(num));
                semop(semid, &(struct sembuf) {(i + 1) % N, +1, 0 }, 1);
                if (num == 0) {
                    return 0;
                }
            }
        }
    }
    while (wait(NULL) > 0) {;}

    lseek(tmp_fd, 0, SEEK_SET);
    unsigned int val;
    while (read(tmp_fd, &val, sizeof(val)) > 0) {
        printf("%u\n", val);
    }

    unlink(filename);
    return 0;
}