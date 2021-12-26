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
    int msgid = msgget(key, 0);
    if (msgid == -1) {;} // чтобы еджадж не ругался

    struct {
        long msgtype;
        char msgtext[5]; // сообщение о том, что запись числа завершена
    } message;

    unsigned int N = strtol(array[1], &array[1] + sizeof(array[1]) - 1, 10);
    int parity =  strtol(array[2], &array[2] + sizeof(array[2]) - 1, 10);

    strcpy(message.msgtext, "done");

    int i; 
    
    if (parity == 0) {
        message.msgtype = 1; 
        // сам себе отправляет сообщение другого типа, чтобы начать
        msgsnd(msgid, (struct msgbuf*) &message, 5, 0);
    }

    for (i = parity; i < N; i = i + 2) {
        msgrcv(msgid, &message, 5, parity + 1, 0); // жду сообщение другого типа (1 или 2)
        printf("%d\n", i);
        fflush(stdout);
        message.msgtype = 2 - parity;
        msgsnd(msgid, (struct msgbuf*) &message, 5, 0);
    }

    return 0;
}

//-------------------------------

int msgid;

void SigHndlr (int s) {
    msgctl(msgid, IPC_RMID, 0);
}

int
main (int argc, char **argv) {

    key_t key = ftok(argv[0], 8); // генерируем ключ

    // создаём очередь сообщений
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == - 1) {
        exit(1);
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

    msgctl(msgid, IPC_RMID, 0);

    return 0;
}