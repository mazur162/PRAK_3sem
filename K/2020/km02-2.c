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

int main(int argc, char** argv)
{
    int fd;
    fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    long long tmp;
    for(int i = 1; i< 6; i++)
    {
        tmp = (long long) i;
        write(fd, &tmp, sizeof(long long));
    }
    close(fd);
  
    fd = open(argv[1], O_RDONLY);
    while(read(fd, &tmp, sizeof(long long)))
    {
        printf("%lld ", tmp);
    }
    printf("\n");
    close(fd);
  
  
    long long first, second;
    long long len, pos=0;
    fd = open(argv[1], O_RDWR);
    len = lseek(fd,0,2)/sizeof(long long);
    lseek(fd,0,0);
    int N;
    sscanf(argv[2], "%d", &N);
    long long back = len - len%N;
    for (int i = 0; i < len/2; i++) {
        if (i % N == 0) {
            read(fd, &first, sizeof(long long));
            pos = lseek(fd,0,1) - sizeof(long long);
            lseek(fd, back*sizeof(long long), 0);
            read(fd, &second, sizeof(long long));
            lseek(fd, -sizeof(long long), 1);
            write(fd, &first, sizeof(long long));
            lseek(fd, pos, 0);
            write(fd, &second, sizeof(long long));
            lseek(fd, pos+ N*sizeof(long long), 0);
            back = back-N;
        }
    }
    close(fd);
  
  
    fd = open(argv[1], O_RDONLY);
    while(read(fd, &tmp, sizeof(long long))) {
        printf("%lld ", tmp);
    }
    printf("\n");
    close(fd);
  
    return 0;
}
