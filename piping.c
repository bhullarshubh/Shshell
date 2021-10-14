#include "headers.h"
void piping(char* command)
{
    char* new[100];
    new[0]=strtok(command,"|");
    int i=0;
    while(new[i]!=NULL)
    {
        i++;
        new[i]=strtok(NULL,"|");
    }
    if(i==0) return;
    else if(i==1)
    {
        int x=tokenize(new[0]);
        execute_redirection(x,0,1);
    }
    else 
    {
        int p[2];
        int input,x;
        input=0;
        for(int j=0;j<=i-2;j++)
        {
            pipe(p);
            x=tokenize(new[j]);
            execute_redirection(x,input,p[1]);
            close(p[1]);
            input=p[0];
        }
        x=tokenize(new[i-1]);
        execute_redirection(x,input,1);
    }
}
int tokenize(char* command)
{
    int j=0;
    args[0]=strtok(command," \t\r");
    while(args[j]!=NULL)
    {
        j++;
        args[j]=strtok(NULL," \t\r");
    }
    return j;
}
void execute_redirection(int x,int input,int output)
{
    int in=dup(0);
    int out=dup(1);
    dup2(input,0);
    dup2(output,1);
    char* temp[4096];
    int j=0;
    for(int i=0;i<x;i++)
    {
        if(strcmp(args[i],"<")==0)
        {
            if(args[i+1]!=NULL)
            {
                int fd=open(args[i+1],O_RDONLY);
                if(fd==-1)
                {
                    dup2(in,0);
                    dup2(out,1);
                    close(in);
                    close(out);

                    printf("ERROR in opening input file specified\n");
                    fflush(stdout);
                    return;
                }
                dup2(fd,0);
                close(fd);
                i++;
            }
            else
            {
                dup2(in,0);
                dup2(out,1);
                close(in);
                close(out);

                printf("No input file specified\n");
                fflush(stdout);
                return;
            }
        }
        else if(strcmp(args[i],">")==0)
        {
            if(args[i+1]!=NULL)
            {
                int fd=open(args[i+1],O_CREAT|O_WRONLY|O_TRUNC,0644);
                if(fd==-1)
                {
                    dup2(in,0);
                    dup2(out,1);
                    close(in);
                    close(out);

                    printf("ERROR in opening output file specified\n");
                    fflush(stdout);
                    return;
                }
                dup2(fd,1);
                close(fd);
                i++;
            }
            else
            {
                dup2(in,0);
                dup2(out,1);
                close(in);
                close(out);

                printf("No output file specified\n");
                fflush(stdout);
                return;
            }
        }
        else if(strcmp(args[i],">>")==0)
        {
            if(args[i+1]!=NULL)
            {
                int fd=open(args[i+1],O_CREAT|O_WRONLY|O_APPEND,0644);
                if(fd==-1)
                {
                    dup2(in,0);
                    dup2(out,1);
                    close(in);
                    close(out);

                    printf("ERROR in opening output file specified\n");
                    fflush(stdout);
                    return;
                }
                dup2(fd,1);
                close(fd);
                i++;
            }
            else
            {
                dup2(in,0);
                dup2(out,1);
                close(in);
                close(out);

                printf("No output file specified\n");
                return;
            }
        }
        else
        {
            temp[j]=args[i];
            j++;
        }
    }
    temp[j]=NULL;
    for(int i=0;i<j;i++)
    {
        token[i]=temp[i];
    }
    token[j]=NULL;
    getflag(j);
    dup2(in, 0);
    dup2(out, 1);
    close(in);
    close(out);
    return;
}