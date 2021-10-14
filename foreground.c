#include "headers.h"
void foreground()  // For foreground process
{
    int pid;
    int status;
    pid=fork();
    current_foreground_id=pid;
    strcpy(current_foreground_name,token[0]);
    if(pid<0)
    {
        printf("Error in forking child\n");
        return;
    }
    else if(pid==0)
    {
        int execute=execvp(token[0],token);
        if(execute<0)
        {
            printf("Command not found\n");
            return;
        }
    }
    else
    {
        waitpid(pid,&status,WUNTRACED);
        current_foreground_id=-1;
    }
}