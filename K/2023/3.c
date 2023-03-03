/* (CMD1 ; CMD2) | (CMD3 || CMD4) >> FILE5 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(int argc, char * argv[])
{
	int fd[2];
	pipe(fd);
	int status;
	
	/* (cmd1 ; cmd2) */
	if (fork() == 0) 
	{ 
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		
		/* cmd1 */
		if (fork() == 0) 
		{
			execlp(argv[1], argv[1], NULL);
			exit(1);
		}
		wait(NULL);
		
		/* cmd2 */
		if (fork() == 0) 
		{
			execlp(argv[2], argv[2], NULL);
			exit(1);
		}
		wait(NULL);
		
		exit(0);
	}
	
	/* (cmd3 || cmd4) */
	if (fork() == 0) 
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		
		/* file5 */
		int f = open(argv[5], O_CREAT | O_APPEND | O_WRONLY, 0666);
		dup2(f, 1);
		close(f);
     
		/* cmd3 */
		if (fork() == 0) 
		{
			execlp(argv[3], argv[3], NULL);
			exit(1);
		}
		wait(&status);
		
		if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0)))
		{
			/* cmd4 */
			if (fork() == 0) 
			{
				execlp(argv[4], argv[4], NULL);
				exit(1);
			}
			wait(NULL);
		}
		
		exit(0);
	}
	
	close(fd[0]);
	close(fd[1]);
	
	wait(NULL);
	wait(NULL);
	
	return 0;
}
