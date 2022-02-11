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
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char** argv)
{
    unsigned a1,a2,a3,a4;
    int pid1, pid2, pid3;
    char c;
    int key = 1234;
    char *shmaddr;
    struct sembuf op;
    int shmid = shmget(key, 256, 0666|IPC_CREAT);
    int semid = semget(key, 3, 0666|IPC_CREAT);
    shmaddr = shmat(shmid, NULL, 0);
    semctl(semid, 0, SETALL, (int)0);
    
    if(!(pid1 = fork()))
    {
        shmid = shmget(key, 256, 0);
        semid = semget(key, 3, 0);
        shmaddr = shmat(shmid, NULL,0);
        op.sem_num = 0;
        op.sem_flg = 0;
        op.sem_op = -1;
        printf("A\n");
        while(semop(semid, &op, 1)!=-1)
        {
            sscanf(shmaddr, "%u %u %u", &a1, &a2, &a3);
            printf("A %u %u %u\n", a1, a2, a3);
            unsigned char a11 = a1;
            a11 = a11 << 1;
            a11 = a11 >> 1;
            printf("A %x\n", a11);
            semop(semid, &op, 1);
        }
        shmdt(shmaddr);
        exit(0);
    }
    if(!(pid2 = fork()))
    {
        shmid = shmget(key, 256, 0);
        semid = semget(key, 3, 0);
        shmaddr = shmat(shmid, NULL,0);
        op.sem_num = 1;
        op.sem_flg = 0;
        op.sem_op = -1;
        printf("B\n");
        while(semop(semid, &op, 1)!=-1)
        {
            sscanf(shmaddr, "%u %u %u", &a1, &a2, &a3);
            printf("B %u %u %u\n", a1, a2, a3);
            unsigned char a11 = a1, a22 = a2;
            a11 = a11 << 2;
            a11 = a11 >> 2;
            printf("B %x%x\n", a11, a22);
            op.sem_op = -1;
            semop(semid, &op, 1);
        }
        shmdt(shmaddr);
        exit(0);
    }
    if(!(pid3 = fork()))
    {
        shmid = shmget(key, 256, 0);
        semid = semget(key, 3, 0);
        shmaddr = shmat(shmid, NULL,0);
        op.sem_num = 2;
        op.sem_flg = 0;
        op.sem_op = -1;
        printf("C\n");
        while(semop(semid, &op, 1)!=-1)
        {
            sscanf(shmaddr, "%u %u %u", &a1, &a2, &a3);
            printf("C %u %u %u\n", a1, a2, a3);
            unsigned char a11 = a1, a22 = a2, a33 = a3;
            a11 = a11 << 3;
            a11 = a11 >> 3;
            printf("C %x%x%x\n", a11, a22, a33);
            op.sem_op = -1;
            semop(semid, &op, 1);
        }
        shmdt(shmaddr);
        exit(0);
    }
    while(scanf("%u%c%u%c%u%c%u", &a1, &c, &a2, &c, &a3, &c, &a4) == 7)
    {
        printf("%u %u %u %u\n", a1, a2, a3, a4);
        sprintf(shmaddr, "%u %u %u", a1, a2, a3);
        if (a1<=127)
        {
            op.sem_num = 0;
            op.sem_flg = 0;
            op.sem_op = 2;
            semop(semid, &op, 1);
            op.sem_op = 0;
            semop(semid, &op, 1);
        }
        else
        {
            if (a1 >=128 && a1<=191)
            {
                op.sem_num = 1;
                op.sem_flg = 0;
                op.sem_op = 2;
                semop(semid, &op, 1);
                op.sem_op = 0;
                semop(semid, &op, 1);
            }
            else
            {
                if (a1 >=192 && a1<=223)
                {
                    op.sem_num = 2;
                    op.sem_flg = 0;
                    op.sem_op = 2;
                    semop(semid, &op, 1);
                    op.sem_op = 0;
                    semop(semid, &op, 1);
                }
            }
        }
    }

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, (int)0);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}