#include "headers.h"
void printprompt()  // for printing prompt
{
    int userid=geteuid();
    struct passwd* currentuser=getpwuid(userid);
    char* name=currentuser -> pw_name;
    char host[100];
    gethostname(host,100);
    char temp[4097];
    char prompt[4096];
    int x=strlen(home);
    getcwd(wd,sizeof(wd));
    sprintf(prompt,"<%s@%s:",name,host);
    if (strcmp(home,wd)==0) strcat(prompt,"~>");  // this is done because dir from where shell is invoked is home directory
    else
    {
        if(strlen(home)<strlen(wd) && strncmp(home,wd,x)==0)
        {
            sprintf(temp,"~/%s>",wd+x+1);
        }
        else
        {
            sprintf(temp,"%s>",wd);
        }
        strcat(prompt,temp);
    }
    printf("%s",prompt);
}