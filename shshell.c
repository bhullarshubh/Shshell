#include "headers.h"
int main()
{
    job_count=0;
    current_foreground_id=-1;
    getcwd(home,sizeof(home));
    getcwd(previouswd,sizeof(previouswd));
    signal(SIGINT,ctrlC);
    signal(SIGTSTP,ctrlZ);
    signal(SIGCHLD,exit_child);
    flag=1;
    while(flag==1)
    {
        printprompt();
        take_input();
        for(int i=0;i<command_count && flag==1;i++)
        {
            piping(command[i]);
            // int no_of_tokens=split_in_tokens(command[i]);
            // getflag(no_of_tokens);
        }
    }
}
void take_input()  //Taking input after prompt printing
{
    char* line;
    long size=0;
    int status=getline(&line,&size,stdin);
    if(status==-1)
    {
        getquit();
        return;
    }
    command_count=0;
    command[command_count]=strtok(line,";\n");
    while(command[command_count] != NULL)     // Dividing input into different commands
    {
        command_count += 1;
        command[command_count]=strtok(NULL,";\n");
    }
}
int split_in_tokens(char* command)   // Commands are separated in tokens,neglecting spaces
{
    int j=0;
    token[0]=strtok(command," \t\r");
    while(token[j] != NULL)
    {
        j += 1;
        token[j]=strtok(NULL," \t\r");
    }
    token[j]=NULL;
    return j;
}
void getflag(int x)   // executing different commands according to our input
{
    if(strcmp(token[0],"echo")==0) getecho(x);
    else if(strcmp(token[0],"pwd")==0) getpwd();
    else if(strcmp(token[0],"cd")==0) getcd(x);
    else if(strcmp(token[0],"ls")==0) getlsflags(x);
    else if(strcmp(token[0],"quit")==0) getquit();
    else if(strcmp(token[0],"pinfo")==0) getpinfo(x);
    else if(strcmp(token[0],"repeat")==0) getrepeat(x);
    else if(token[x-1][0] == '&') background(x);
    else if(strcmp(token[0],"jobs")==0) getjobs(x);
    else if(strcmp(token[0],"fg")==0) getfg(x);
    else if(strcmp(token[0],"bg")==0) getbg(x);
    else if(strcmp(token[0],"sig")==0) getsig(x);
    else foreground();
}