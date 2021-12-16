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
    int semid = semget(key, 0, 0);
    if (semid == -1) {;} // чтобы еджадж не ругался
    int shmid = shmget(key, 0, 0);
    char *shmaddr = shmat(shmid, NULL, 0);
    if (*shmaddr == -1) {;} // чтобы еджадж не ругался
    int msgid = msgget(key, 0);
    if (msgid == -1) {;} // чтобы еджадж не ругался

    struct {
        long msgtype;
        char* msgtext; // сообщение о том, что запись числа завершена
    } message;

    unsigned int N = strtol(array[1], &array[2], 10);
    int parity = strtol(array[2], &array[2] + sizeof(array[2]) - 1, 10); /////////
    printf("%d\n", parity);
    int i;

    message.msgtext = "done\0";
    message.msgtype = parity;
    
    struct sembuf oper[4] = {{0, -1, 0}, {0, +1, 0}, {1, -1, 0}, {1, +1, 0}};
    for (i = parity; i < N; i = i + 2) {
        msgrcv(msgid, &message, 5, 0, 0);
        semop(semid, oper + 2 * parity, 1); // down(sem0) // down(sem1)
        printf("%d\n", i);
        fflush(stdout);
        semop(semid, oper + 3 - 2 * parity, 1); // up(sem1) // up(sem0)
        msgsnd(msgid, &message, 5, 0);
    }

    return 0;
}

//-------------------------------

int semid, msgid, shmid;
char * shmaddr;

void SigHndlr (int s) {
    semctl(semid, 2, IPC_RMID);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
    msgctl(msgid, IPC_RMID, 0);
}

int
main (int argc, char **argv) {
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;
    arg.val = 0;

    key_t key = ftok(argv[0], 8); // генерируем ключ

    // создаём 2 семафора с необходимыми правами доступа
    semid = semget(key, 2, 0666 | IPC_CREAT);
    if (semid == -1) {
        exit(1);
    }

    // создаём разделяемую память на 1024 элемента
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid == -1) {
        semctl(semid, 2, IPC_RMID);
        exit(1);
    }

    // подключаемся к разделу памяти
    shmaddr = shmat(shmid, NULL, 0); // в shmaddr указатель на буфер

    // создаём очередь сообщений
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == - 1) {
        semctl(semid, 2, IPC_RMID);
        shmdt(shmaddr); // отключаемся от разделяемой памяти
        shmctl(shmid, IPC_RMID, 0); // уничтожаем разделяемую память
        exit(1);
    }

    // инициализация семафоров
    for (int i = 0; i < 1; i++) {
        if (semctl(semid, i, SETVAL, arg.buf) == -1) {
            semctl(semid, 2, IPC_RMID);
            shmdt(shmaddr);
            shmctl(shmid, IPC_RMID, 0);
            msgctl(msgid, IPC_RMID, 0);
            exit(1);
        }
    }

    char** array;
    array[0] = argv[0];
    array[1] = argv[1];
    pid_t pid0 = fork();
    if (pid0 == 0) {
        array[2] = "0\0"; // этот сын будет выводить чётные числа
        return son_func(argc + 1, array);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        array[2] = "1\0"; // этот сын будет выводить нечётные числа
        return son_func(argc + 1, array);
    }

    signal(SIGINT, SigHndlr);

    wait(NULL);
    wait(NULL);
    semctl(semid, 2, IPC_RMID);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
    msgctl(msgid, IPC_RMID, 0);

    return 0;
}