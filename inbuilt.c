#include "headers.h"
void getrepeat(int y)  // for repeat command
{
    int w=atoi(token[1]);
    for(int i=0;i<y;i++)
    {
        token[i]=token[i+2];
    }
    for(int j=0;j<w;j++)
    {
        getflag(y-2);
    }
    flag=1;
}
void getecho(int y)  // for echo command
{
    for(int i=1;i<y;i++)
    {
        printf("%s ",token[i]);
    }
    printf("\n");
    flag=1;
}
void getpwd()  // for pwd command
{
    printf("%s\n",wd);
    flag=1;
}
void getquit()   // for quit command
{
    printf("EXIT SHELL\n");
    flag=0;
}
void getcd(int y)  // for cd command
{
    if(y==1 || strcmp(token[1],"~")==0)
    {
        strcpy(previouswd,wd);
        chdir(home);
    }
    else if(strcmp(token[1],"-")==0)
    {
        chdir(previouswd);
        printf("%s\n",previouswd);
        strcpy(previouswd,wd);
    }
    else
    {
        strcpy(previouswd,wd);
        chdir(token[1]);
    }
    flag=1;
}
void getlsflags(int y)  // for taking input of diff flags and directories in ls
{
    char* dir[10];
    int lflag=0;
    int aflag=0;
    int j=0;
    dir[0]=".";
    for(int i=1;i<y;i++)
    {
        if(token[i]==NULL || strcmp(token[i],".")==0)
        {
            break;
        }
        else if(token[i][0]=='-')
        {
            if(strlen(token[i])==2)
            {
                if(token[i][1]=='l') lflag=1;
                if(token[i][1]=='a') aflag=1;
            }
            if(strlen(token[i])==3)
            {
                if((token[i][1]=='l' && token[i][2]=='a') || (token[i][1]=='a' && token[i][2]=='l'))
                {
                    lflag=1;
                    aflag=1;
                }
            }
        }
        else
        {
            dir[j]=token[i];
            j++;
        }
    }
    if(strcmp(dir[0],"~")==0 || strcmp(dir[0],".")==0 )
    {
        dir[0]=home;
    }
    if(j==0) j=1;
    for(int i=0;i<j;i++)
    {
        getls(dir[i],aflag,lflag);
        printf("\n");
    }
}
void getls(char* dir_name,int x,int y)  // here actual ls is done
{
    struct dirent** files;
    int file_number=0;
    if(x==1 && y==0)
    {
        file_number=scandir(dir_name,&files,NULL,alphasort);
        if(file_number==-1)
        {
            perror("ERROR\n");
        }
        for(int i=0;i<file_number;i++)
        {
            printf("%s\t",files[i]->d_name);
        }
    }
    if(x==1 && y==1)
    {
        file_number=scandir(dir_name,&files,NULL,alphasort);
        if(file_number==-1)
        {
            perror("ERROR\n");
        }
        for(int i=0;i<file_number;i++)
        {
            char filepath[4096];
            strcpy(filepath,dir_name);
            strcat(filepath,"/");
            strcat(filepath,files[i]->d_name);
            getpermission(filepath);
            struct stat filevalues;
            int stat_value=stat(filepath,&filevalues);
            if(stat_value==-1)
            {
                perror("ERROR in getting status of files");
            }
            int hardlink=filevalues.st_nlink;
            int lsuid=filevalues.st_uid;
            int lsgid=filevalues.st_gid;
            int size=filevalues.st_size;
            char date[50];
            strftime(date,50, "%Y %m %d %H:%M",localtime(&(filevalues.st_ctime)));
            printf("%s %d %s %s %d %s %s\n",permission,hardlink,getpwuid(lsuid)->pw_name,getgrgid(lsgid)->gr_name,size,date,files[i]->d_name);
        }
    }
    if(x==0 && y==1)
    {
       file_number=scandir(dir_name,&files,NULL,alphasort);
        if(file_number==-1)
        {
            perror("ERROR\n");
        }
        for(int i=0;i<file_number;i++)
        {
            if((files[i]->d_name)[0] != '.')
            {
                char filepath[4096];
                strcpy(filepath,dir_name);
                strcat(filepath,"/");
                strcat(filepath,files[i]->d_name);
                getpermission(filepath);
                struct stat filevalues;
                int stat_value=stat(filepath,&filevalues);
                if(stat_value==-1)
                {
                    perror("ERROR in getting status of files");
                }
                int hardlink=filevalues.st_nlink;
                int lsuid=filevalues.st_uid;
                int lsgid=filevalues.st_gid;
                int size=filevalues.st_size;
                char date[50];
                strftime(date,50, "%Y %m %d %H:%M",localtime(&(filevalues.st_ctime)));
                printf("%s %d %s %s %d %s %s\n",permission,hardlink,getpwuid(lsuid)->pw_name,getgrgid(lsgid)->gr_name,size,date,files[i]->d_name);
            }
        } 
    }
    if(x==0 && y==0)
    {
        file_number=scandir(dir_name,&files,NULL,alphasort);
        if(file_number==-1)
        {
            perror("ERROR\n");
        }
        for(int i=0;i<file_number;i++)
        {
            if((files[i]->d_name)[0] != '.')
            {
                printf("%s\t",files[i]->d_name);
            }
        }
    }
}
void getpermission(char* filepath)
{
    struct stat filevalues;
    int stat_value=stat(filepath,&filevalues);
    if(stat_value==-1)
    {
        perror("ERROR in getting status of files");
    }
    if (S_ISDIR(filevalues.st_mode))
        permission[0]='d';
    else if (S_ISLNK(filevalues.st_mode))
        permission[0]='l';
    else
        permission[0]='-';
    permission[1]=(filevalues.st_mode & S_IRUSR) ? 'r' : '-';
    permission[2]=(filevalues.st_mode & S_IWUSR) ? 'r' : '-';
    permission[3]=(filevalues.st_mode & S_IXUSR) ? 'r' : '-';
    permission[4]=(filevalues.st_mode & S_IRGRP) ? 'r' : '-';
    permission[5]=(filevalues.st_mode & S_IWGRP) ? 'r' : '-';
    permission[6]=(filevalues.st_mode & S_IXGRP) ? 'r' : '-';
    permission[7]=(filevalues.st_mode & S_IROTH) ? 'r' : '-';
    permission[8]=(filevalues.st_mode & S_IWOTH) ? 'r' : '-';
    permission[9]=(filevalues.st_mode & S_IXOTH) ? 'r' : '-';
    permission[10]='\0';
}
void getpinfo(int y)  // for pinfo
{
    int pid;
    if(y==1) pid=getpid();
    else pid=atoi(token[1]);
    char filepath[1000];
    sprintf(filepath,"/proc/%d/stat",pid);
    FILE* stat;
    stat=fopen(filepath,"r");
    if(stat==NULL)
    {
        perror("error in opening stat file\n");
        return;
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
    int tpgid=atoi(parsed_token[7]);
    printf("PID -- %d\n",pid);
    printf("Process Status -- %s",parsed_token[2]);
    if(pid==tpgid) printf("+\n");
    else printf("\n");
    printf("Memory -- %s\n",parsed_token[22]);
    sprintf(filepath,"/proc/%d/exe",pid);
    char exe_path[1000];
    char real_path[1000];
    int rl=readlink(filepath,exe_path,sizeof(exe_path));
    if(rl==-1)
    {
        perror("Error in reading from exe\n");
        return;
    }
    if(strlen(exe_path)>strlen(home) && strncmp(home,exe_path,strlen(home))==0)
    {
        sprintf(real_path,"~/%s",exe_path+strlen(home)+1);
    }
    else
    {
        strcpy(real_path,exe_path);
    }
    printf("Executable Path -- %s\n",real_path);
    flag=1;
}