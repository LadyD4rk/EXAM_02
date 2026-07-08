#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int    picoshell(char **cmds[])
{
    pid_t pid;
    int fd[2];
    int last_fd = -1;
   // int status;  // for wati();
    //int exit_code = 0; //final return value
    int i = 0;

    while(cmds[i])
    {
        if(cmds[i + 1] && pipe(fd) == -1)
            return 1;
        pid = fork();
        if(pid == -1)
        {
            if(cmds[i + 1])
            {
                close(fd[0]);
                close(fd[1]);
            }
            return 1;
        }
        if(pid == 0)
        {
            if(last_fd != -1)
            {
                if(dup2(last_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(last_fd);
            }
            if(cmds[i + 1])
            {
                close(fd[0]);
                if(dup2(fd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(fd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        if(last_fd != -1)   //if not the first command, close the previous pipe's read end 
            close(last_fd);  // it's no longer needed in the parent
        if(cmds[i + 1])  // if there is a next command
        {
            close(fd[1]); // close the write end of the current pipe in the parent
            last_fd = fd[0];  // save the read end for the next command
        }
        i++;
    }
    // int last_status= 0;
    // while(wait(&status) != -1)  //wait for all child processes to finish
    //     last_status = status; 
        
    //     if ( WIFEXITED(last_status)) //check if child exited with error
    //         return WEXITSTATUS(last_status);
    //     else
    //         return 1; //if all commands succeeded, set exit_code to 0
    // return exit_code; //return 0 if all commands succeeded, 1 if any failed
    /* while (wait(&status) != -1)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            exit_code = 1;
    }
    
    return exit_code;*/
    while(wait(NULL) > 0)
    { 
    	;
    }
    return(0);
}
