#include "headers.h"
void background(int y)   // for background process
{
    token[y-1]=NULL;
    if(strcmp(token[0],"vim")==0)
    {
        printf("Use command other than vim\n");
        return;
    }
    int pid=fork();
    if(pid<0)
    {
        perror("Error in child forking\n");
        return;
    }
    if(pid==0)
    {
        printf("%d\n",getpid());
        fflush(stdout);
        int execute=execvp(token[0],token);
        if(execute<0)
        {
            printf("Command not found\n");
            return;
        }
    }
    else
    {
        job_count++;
        array[job_count-1].pid=pid;
        array[job_count-1].count=job_count;
        array[job_count-1].present=1;
        strcpy(array[job_count-1].name,token[0]);
        signal(SIGCHLD,exit_child);
    }
    flag=1;
}
void exit_child(int signum)  // for printing according to status after child ended 
{
    int wait_id;
    int status;
    while((wait_id=waitpid(-1,&status,WNOHANG))>0)
    {
        if(WIFEXITED(status))
        {
	    	fprintf(stderr,"\nProcess with pid %d exited normally\n", wait_id);
            remove_job(wait_id);
            printprompt();
            fflush(stdout);
	    }
	    if(WIFSIGNALED(status))
        {
	    	fprintf(stderr,"\nProcess with pid %d exited due to a signal given by user\n", wait_id);
            remove_job(wait_id);
            printprompt();
            fflush(stdout);
        }
    }
}