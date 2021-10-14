#include "headers.h"
void getjobs(int y)
{
    if (y==2)
    {
        if(strcmp(token[1],"-r")!=0 && strcmp(token[1],"-s")!=0)
        {
            printf("Error:Wrong Flag\n");
            return;
        }
    }
    qsort(array,job_count,sizeof(struct jobs),compare);
    char filepath[1000];
    for(int i=0;i<job_count;i++)
    {
        if(array[i].present==1)
        {
            int id=array[i].pid;
            sprintf(filepath,"/proc/%d/stat",id);
            FILE* stat;
            stat=fopen(filepath,"r");
            if(stat==NULL)
            {
                continue;
            }
            char* line;
            long size=0;
            int status=getline(&line,&size,stat);
            if(status==-1)
            {
                perror("Error in reading line\n");
                return;
            }
            char* parsed_token[100];
            int j=0;
            parsed_token[0]=strtok(line," ");
            while(parsed_token[j] != NULL)
            {
                j += 1;
                parsed_token[j]=strtok(NULL," ");
            }
            if(y==1)
            {
                if(parsed_token[2][0]=='R' || parsed_token[2][0]=='S') printf("[%d] Running %s [%d]\n",array[i].count,array[i].name,id);
                else printf("[%d] Stopped %s [%d]\n",array[i].count,array[i].name,id);
            }
            if(y==2)
            {
                if(strcmp(token[1],"-r")==0)
                {
                    if(parsed_token[2][0]=='R' || parsed_token[2][0]=='S') printf("[%d] Running %s [%d]\n",array[i].count,array[i].name,id);
                }
                else if(strcmp(token[1],"-s")==0)
                {
                    if(parsed_token[2][0]=='T') printf("[%d] Stopped %s [%d]\n",array[i].count,array[i].name,id);
                }
            }
        }
    }
}
int compare(const void* p,const void* q)
{
    return strcmp(((struct jobs*) p)->name,((struct jobs*) q)->name);
}
void getfg(int y)
{
    if (y != 2)
    {
        printf("Error:Wrong number of arguments\n");
        return;
    }
    int id=atoi(token[1]);
    for(int i=0;i<job_count;i++)
    {
        if(array[i].present==1)
        {
            if(id==array[i].count)
            {
                int status;
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU,SIG_IGN);
                current_foreground_id=array[i].pid;
                strcpy(current_foreground_name,array[i].name);
                kill(array[i].pid,SIGCONT);
                remove_job(array[i].pid);
                waitpid(array[i].pid, &status, WUNTRACED);
                if(WIFSTOPPED(status))
                {
                    if(current_foreground_id!=-1)
                    {
                        job_count++;
                        array[job_count-1].pid=current_foreground_id;
                        array[job_count-1].present=1;
                        strcpy(array[job_count-1].name,current_foreground_name);
                        array[job_count-1].count=job_count;
                    }
                    current_foreground_id=-1;
                }
                signal(SIGTTIN,SIG_DFL);
                signal(SIGTTOU,SIG_DFL);
                return;
            }
        }
    }
    printf("Error:No job with such job number found\n");
    return;
}
void getbg(int y)
{
    if (y != 2)
    {
        printf("Error:Wrong number of arguments\n");
        return;
    }
    int id=atoi(token[1]);
    for(int i=0;i<job_count;i++)
    {
        if(array[i].present==1)
        {
            if(id==array[i].count)
            {  
                kill(array[i].pid,SIGCONT);
                return;
            }
        }
    }
    printf("Error:No job with such job number found\n");
    return;
}
void getsig(int y)
{
    if(y!=3)
    {
        printf("Error:Wrong number of arguments\n");
        return;
    }
    int id=atoi(token[1]);
    int signum=atoi(token[2]);
    for(int i=0;i<job_count;i++)
    {
        if(id==array[i].count)
        {
            int x=kill(array[i].pid,signum);
            if(x==-1)
            {
                perror("Error in sending signal\n");
                return;
            }
            return;
        }
    }
    printf("Error:No such job with this job number exists\n");
    return;
}
void remove_job(int id)
{
    for(int i=0;i<job_count;i++)
    {
        if(id==array[i].pid)
        {
            array[i].present=0;
        }
    }
    return;
}
void ctrlC(int signum)
{
    if(current_foreground_id==-1)
    {
        printprompt();
        fflush(stdout);
    }
    else
    {
        kill(current_foreground_id,SIGINT);
        current_foreground_id=-1;
    }
}
void ctrlZ(int signum)
{
    if(current_foreground_id==-1)
    {
        printprompt();
        fflush(stdout);
    }
    else
    {
        kill(current_foreground_id,SIGTSTP);
        job_count++;
        array[job_count-1].pid=current_foreground_id;
        array[job_count-1].count=job_count;
        array[job_count-1].present=1;
        strcpy(array[job_count-1].name,current_foreground_name);
        current_foreground_id=-1;
        return;
    }
    return;
}