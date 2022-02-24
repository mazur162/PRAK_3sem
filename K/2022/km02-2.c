#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
   int k = ftok(argv[0], 0);
   int semid = semget(k, 3, 0666 | IPC_CREAT | IPC_EXCL);
   int shmid = shmget(k, 2 * sizeof(unsigned), 0666 | IPC_CREAT | IPC_EXCL); // создаём разделяемую память
   unsigned *p = shmat(shmid, NULL, 0);
   p[1] = 0;
   semctl(semid, 0, SETVAL, 1);
   semctl(semid, 1, SETVAL, 0);
   semctl(semid, 2, SETVAL, 0);
   unsigned u;
   if (fork() == 0) {
      FILE *f = fopen(argv[1], "r");
      unsigned odd = 1;
      while (fscanf(f, "%u", &u) > 0) {
         struct sembuf dn = {0, -1, 0};
         semop(semid, &dn, 1);
         *p = u;
         if (odd) {
            struct sembuf up = {1, 1, 0};
            semop(semid, &up, 1);
         } else {
            struct sembuf up = {2, 1, 0};
            semop(semid, &up, 1);
         }
         odd ^= 1;
      }
      p[1] = 1;
      fclose(f);
      struct sembuf up[] = {{1, 1, 0},{2, 1, 0}};
      semop(semid, up, 2);
      shmdt(p); // не обязательно, но культурнее
      exit(0);
   }

   for (int i = 1; i <= 2; i++) {
      if (fork() == 0) {
         while (1) {
            struct sembuf dn = {i, -1, 0};
            semop(semid, &dn, 1);
            if (p[1]) {
               break;
            }
            if (i == 2) {
               *p *= 2;
            } else {
               *p += 1;
            }
            printf("%u\n", *p);
            fflush(stdout);
            struct sembuf up = {0, 1, 0};
            semop(semid, &up, 1);
         }
      }
      shmdt(p); // не обязательно, но культурнее
      exit(0);
   }

}
while (wait(NULL) > 0);
shmdt(p); // обязательно!
semctl(semid, IPC_RMID, 0);
shmctl(shmid, IPC_RMID, NULL);
return 0;
}

