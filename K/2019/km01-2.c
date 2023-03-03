#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(int argc, char * argv[]) //p0
{
	int fd32[2], fd21[2], fd13[2];
	long long a = 1, b = 1, n = atoll(argv[1]);
	int sll = sizeof(long long); 
	
	pipe(fd32);
	pipe(fd21);
	pipe(fd13);

	if (fork() == 0) // p1
	{
		if (fork() == 0) //p3
        {
            close(fd21[1]);
            close(fd21[0]);
            close(fd13[1]);
            close(fd32[0]);
            while (1) 
            {
                read(fd13[0], &a, sll);
                read(fd13[0], &b, sll);
  
                if (b > n) 
                {
                    write(fd32[1], &a, sll);
                    write(fd32[1], &b, sll);
                    close(fd13[0]);
                    close(fd32[1]);
                    break;
                }
                printf("3 ");
                printf("%lld %lld\n", a, b);
                a = a + b;
                write(fd32[1], &b, sll);
                write(fd32[1], &a, sll);
            }
            exit(0);
        }
        
		close(fd32[0]);
        close(fd32[1]);
        close(fd21[1]);
        close(fd13[0]);
        
		while (1) 
		{
			read(fd21[0], &a, sll);
			read(fd21[0], &b, sll);

			if (b > n) 
			{
				write(fd13[1], &a, sll);
                write(fd13[1], &b, sll);
                close(fd21[0]);
                close(fd13[1]);
				break;
			}
			printf("1 ");
			printf("%lld %lld\n", a, b);
			a = a + b;
			write(fd13[1], &b, sll);
            write(fd13[1], &a, sll); 
        }
		wait(NULL);
		exit(0);
	}
	
	if (fork() == 0) // p2
	{
		close(fd13[0]);
        close(fd13[1]);
        close(fd32[1]);
        close(fd21[0]);
        while (1) 
        {
            read(fd32[0], &a, sll);
            read(fd32[0], &b, sll);
            if (b > n) 
            {
                write(fd21[1], &a, sll);
                write(fd21[1], &b, sll);
                close(fd32[0]);
                close(fd21[1]);
                break;
            }
            printf("2 ");
            printf("%lld %lld\n", a, b);
            a = a + b;
            write(fd21[1], &b, sll);
            write(fd21[1], &a, sll);
        }
        exit(0);
    }
	
	write(fd13[1], &a, sll);
    write(fd13[1], &b, sll);
    
	close(fd13[0]);
    close(fd13[1]);
    close(fd21[0]);
    close(fd21[1]);
    close(fd32[0]);
    close(fd32[1]);
    
	wait(NULL);
	wait(NULL);
	
	return 0;
}
