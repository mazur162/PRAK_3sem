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

int
son_func (int argc, char **argv) {
    key_t key = ftok(argv[0], 8);
    int semid = semget(key, 0, 0);
    if (semid == -1) {;} // чтобы еджадж не ругался
    int shmid = shmget(key, 0, 0);
    char *shmaddr = shmat(shmid, NULL, 0);
    if (*shmaddr == -1) {;} // чтобы еджадж не ругался

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

    pid_t pid = fork();
    if (pid == 0) {
        return son_func(argc, argv);
    }

    signal(SIGINT, SigHndlr);

    wait(NULL);
    semctl(semid, 2, IPC_RMID);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
    msgctl(msgid, IPC_RMID, 0);

    return 0;
}