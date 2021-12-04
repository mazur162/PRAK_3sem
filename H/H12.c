#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct { 
    long mtype;
    char Data[258];
} Message;

int main(int argc, char **argv) {
    int key;
    int msgid;
    char str[258];
    key = IPC_PRIVATE;
    msgid = msgget(key, 0666 | IPC_CREAT);

    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) {
        for (;;) {
            msgrcv(msgid, (struct msgbuf*) (&Message), 258, 1, 0);  
            if (Message.Data[0]=='q' || Message.Data[0]=='Q') 
                break;
            printf("A: %s", Message.Data);
        }
        return 0;
    }
    if ((pid2 = fork()) == 0) {
        for (;;) {
            msgrcv(msgid, (struct msgbuf*) (&Message), 258, 2, 0);  
            if (Message.Data[0]=='q' || Message.Data[0]=='Q') 
                break;
            printf("B: %s", Message.Data);
        }
        return 0;
    }
    for (;;) {
        fgets(str, 258, stdin);
        strcpy(Message.Data, str);
        switch(str[0]) { 
            case 'a':
            case 'A':
                Message.mtype = 1;
                msgsnd(msgid, (struct msbuf*)(&Message), strlen(str) + 1, 0);
                break;
            case 'b':
            case 'B':
                Message.mtype = 2;
                msgsnd(msgid, (struct msbuf*)(&Message), strlen(str) + 1, 0);
                break;
            case 'q':
            case 'Q':
                Message.mtype = 1;
                msgsnd(msgid, (struct msbuf*)(&Message), strlen(str) + 1, 0);
                Message.mtype = 2;
                msgsnd(msgid, (struct msbuf*)(&Message), strlen(str) + 1, 0);
                wait(NULL);
                wait(NULL);
                
                msgctl(msgid, IPC_RMID, NULL);

                return 0;
            default:
                break;
        }
    }
    return 0;
}