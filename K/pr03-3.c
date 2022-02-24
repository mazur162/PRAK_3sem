#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// { cmd1 || cmd2 } && cmd3
// Запись a || b означает то же самое, что !a?b:true. Запись a && b означает то же самое, что a?b:false. Скобки { и } используются для группировки.

int 
main(int argc, char** argv) {
    int status;

    if (!fork()) {
        if (!fork()) {
            execlp(argv[1], argv[1], NULL);
            return 127;
        }
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 0;
        }
        
        if (!fork()) {
            execlp(argv[2], argv[2], NULL);
            return 127;
        }
        wait(&status);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }

        return WTERMSIG(status);
    }
    wait(&status);

    if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status)) {
        return 1;
    }
    
    if (!fork()) {
        execlp(argv[3], argv[3], NULL);
        return 127;
    }
    wait(&status);

    if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status)) {
        return 1;
    }

    return 0;
}