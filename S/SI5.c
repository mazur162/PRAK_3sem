#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <string.h>


int
son_func (int num_of_args, char **array) {
    key_t key = ftok(array[0], 8);
    unsigned int N = strtol(array[1], NULL, 10);
    int parity =  strtol(array[2], NULL, 10);

    int semid = semget(key, 0, 0);
    if (semid == -1) {;} // чтобы еджадж не ругался
    int shmid = shmget(key, N*sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {;} // чтобы еджадж не ругался
    int *shmaddr = (int*)shmat(shmid, NULL, 0);
    if (shmaddr == (int*)-1) {;} // чтобы еджадж не ругался

    struct sembuf oper[2] = {{0, -1, 0}, {0, +1, 0}};

    int sign = (parity * 2) - 1;
    // чётный сын будет идти вниз по циклу, нечётный вверх

    shmaddr[N] = N - 1;
    shmaddr[N + 1] = 0;
    
    int i = (N - 1) - parity * (N - 1); 
    
    for (int i = (N - 1) - parity * (N - 1); i != parity * (N - 1) + sign; i = i + sign) {
        if ((i != 0) && (i == shmaddr[N + 1 - parity])) {
            shmdt(shmaddr);
            return 0;
        }

        semop(semid, oper, 1); // down(sem)
        shmaddr[i]++;
        shmaddr[N + parity] = i;
        semop(semid, oper + 1, 1); //up(sem)
        printf("%d) %d; [N] = %d, [N+1] = %d\n", parity, i, shmaddr[N], shmaddr[N+1]);

        

    };

    shmdt(shmaddr);
    return 0;
}

//-------------------------------

int semid, shmid;
int *shmaddr;

void SigHndlr (int s) {
    semctl(semid, 1, IPC_RMID);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
}

int
main (int argc, char **argv) {
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

    key_t key = ftok(argv[0], 8); // генерируем ключ
    unsigned int N = strtol(argv[1], NULL, 10);

    // создаём 1 семафоров с необходимыми правами доступа
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        exit(1);
    }

    // инициализация 1 семафора
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        semctl(semid, 1, IPC_RMID);
        exit(1);
    }

    // создаём разделяемую память на (N + 2) элемента
// в последних двух элементах хранятся индекс 
//последнего изменного числа каждого сына соотвественно
    shmid = shmget(key, (N + 2) * sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        semctl(semid, 2, IPC_RMID);
        exit(1);
    }

    // подключаемся к разделу памяти
    shmaddr = (int*)shmat(shmid, NULL, 0); // в shmaddr указатель на буфер

    for (int i = 0; i < N; i++) {
        shmaddr[i] = i;
    }

    char* array[argc + 1];
    array[0] = argv[0];
    array[1] = argv[1];
    pid_t pid0 = fork();
    if (pid0 == 0) {
        char str[2];
        sprintf(str, "%s", "0");
        array[2] = str; // этот сын будет выводить чётные числа
        return son_func(argc + 1, array);
    }
 
    pid_t pid1 = fork();
    if (pid1 == 0) {
        char str[2];
        sprintf(str, "%s", "1");
        array[2] = str; // этот сын будет выводить нечётные числа
        return son_func(argc + 1, array);
    }

    signal(SIGINT, SigHndlr);

    wait(NULL);
    wait(NULL);

    if (N < 10000) {
        for (int i = 0; i < N; i++) {
            printf("%d\n", shmaddr[i]);
        }
    }

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 1, IPC_RMID);

    return 0;
}