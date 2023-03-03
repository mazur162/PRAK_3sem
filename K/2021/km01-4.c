// ( cmd1 < file1 && cmd2 ) | cmd3 >> file2
// cmd1 cmd2 cmd3 file1 file2

int 
main(int argc, char *argv[]) 
{
    int status;
    int fd[2];
    
    pipe(fd);
    
    /* brackets */
    if (fork() == 0) 
    {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]); 
        
		/* gson1 */
        if (fork() == 0) 
        { 
            int file_in = open(argv[4], O_RDONLY);
            dup2(file_in, 0);
            close(file_in);
            
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }
        wait(&status);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
        {
           /* gson2 */
           if (fork() == 0)
           { 
               execlp(argv[2], argv[2], NULL);
               exit(127);
           }
           wait(&status); 
        }
        
        exit(0);
    }

    close(fd[1]);
    
    /* son2 */
    if (fork() == 0) 
    {
        int file_out;
        dup2(fd[0], 0);
        close(fd[0]);
        
        file_out = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0666);
        dup2(file_out, 1);
        close(file_out);
        
        execlp(argv[3], argv[3], NULL);
        exit(127);
    }
    
    close(fd[0]);
    
    wait(&status);
    wait(&status);
    
    return 0;
}
