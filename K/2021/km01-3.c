#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>


int 
main(int argc, char **argv) 
{
    // int status;
    int fd1[2], fd2[2];
    
    pipe(fd1);
    pipe(fd2);

    if (fork() == 0)
    {
        dup2(fd1[0], 0);
        close(fd1[0]);
        close(fd1[1]);

        close(fd2[0]);
        dup2(fd2[1], 1);
        close(fd2[1]);

        // execlp(argv[2], argv[2], NULL);
        int q;
        char buf[16];
        unsigned long long n = 0, i;
        while (q = read(0, buf, 16))
        {
            for (i = 0; i < q; i++)
            {
                if (buf[i] == ' ')
                {
                    write(1, &n, sizeof(unsigned long long));
                    n = 0;
                }
                else
                {
                    n = n * 10 + buf[i] - '0';
                }
            }
        }
        write(1, &n, sizeof(unsigned long long));
        exit(0);
    }


    close(fd1[0]);
    close(fd2[1]);

    char buf[20];
    // 19 18 17 => len (=3)
    // write(fd1[1], buf+20-len)


    unsigned long long i;
    unsigned long long M = atoll(argv[1]);
    unsigned long long k;
    int len;
    char g = ' ';

    for (i = 0; i < M-1; i++)
    {
        for (k = 0; k < 20; k++)
        {
            buf[k] = 0;
        }

        k = i * i;
        len = 1;
        buf[20-len] = k % 10 + '0';
        k = k / 10;
        
        while (k) 
        {
            len++;
            buf[20-len] = k % 10 + '0';
            k = k / 10;
        }

        write(fd1[1], ((char*) buf)+20-len, len);
        write(fd1[1], &g, 1);


        // dprintf(fd1[1], "%llu ", i*i);

    }

    // dprintf(fd1[1], "%llu", i*i);
    for (k = 0; k < 20; k++)
        {
            buf[k] = 0;
        }
    k = i * i;
    len = 1;
    buf[20-len] = k % 10 + '0';
    k = k / 10;
    
    while (k) 
    {
        len++;
        buf[20-len] = k % 10 + '0';
        k = k / 10;
    }
    write(fd1[1], ((char*) buf)+20-len, len);

    close(fd1[1]);

    // int q;
    // char buf[16];
    // unsigned long long n = 0;

    // while (q = read(fd2[0], buf, 16))
    // {
    //     for (i = 0; i < q; i++)
    //     {
    //         if (buf[i] == ' ')
    //         {
    //             printf("%llu\n", n);
    //             n = 0;
    //         }
    //         else
    //         {
    //             n = n * 10 + buf[i] - '0';
    //         }
    //     }
    // }
    // close(fd2[0]);

    unsigned long long s = 0;
    while (read(fd2[0], &i, sizeof(unsigned long long)))
    {
        s += i;
        
    }
    printf("%llu\n", s);
    close(fd2[0]);

    // printf("QQ\n");

    wait(NULL);




    //     dup2(fd[1], 1); // 
    //     close(fd[1]);
        
    //     exit(127);
    //     }

    //     wait(&status);
    //     if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
    //     {
    //        if (fork()==0) // gson2
    //        { 
    //            dup2(fd[1], 1);
    //            close(fd[1]);
    //            execlp(argv[2], argv[2], NULL);
    //            exit(127);
    //        }
    //     }
    //     close(fd[1]);  // проверять !!!

    //     wait(&status); // while (wait(NULL) > 0) {;}
    //     exit(0);
    // }

    //     pipe()

    // close(fd[1]);// особое внимание
    // if (fork()==0) {  // son2
    //     int file_out;
    //     dup2(fd[0], 0);
    //     close(fd[0]);
    //     file_out = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0666);
    //     dup2(file_out, 1);
    //     close(file_out);
    //     execlp(argv[3], argv[3], NULL);
    //     exit(127);
    // }
    // close(fd[0]);
    
    
    // wait(&status); // while (wait(NULL) > 0) {;}
    // wait(&status);
    return 0;
}
