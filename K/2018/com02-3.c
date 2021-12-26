#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int counter = 0;

void sighndlr(int s) {
	signal(SIGUSR1, sighndlr);
	signal(SIGUSR2, sighndlr);
	if (s == SIGUSR1) {
		counter += 5;
	} 
	
	if(s == SIGUSR2) {
		counter -= 4;
	}
	printf("%d %d\n", s, counter);
	fflush(stdout);
}

int main (void) {
	signal(SIGUSR1, sighndlr);
	signal(SIGUSR2, sighndlr);
	
	printf("%d\n", getpid());
	
	pid_t p;
	if ((p = fork()) == 0) {
		while (1) {
			usleep(123456);
			if (counter < 0) {
				break;
			}
		}
		exit(0);
	}
	kill(p, SIGUSR1);
	usleep(123456);
	kill(p, SIGUSR2);
	usleep(123456);
	kill(p, SIGUSR2);
	
	int status;
	wait(&status);
	if(WIFEXITED(status)) {
		printf("%d\n", 	WEXITSTATUS(status));
	}
	if (WIFSIGNALED(status)) {
		printf("%d\n", 	128 + WTERMSIG(status));
	}
	return 0;
}


