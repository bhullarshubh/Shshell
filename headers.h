#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
char home[4096];
char wd[4096];
char previouswd[4096];
char* command[100];
char* args[100];
char* token[4096];
int command_count;
int flag;
char permission[10];
typedef struct jobs
{
    char name[1000];
    int pid;
    int count;
    int present;
} job;
job array[10000];
int current_foreground_id;
char current_foreground_name[1000];
int job_count;
void take_input();
int split_in_tokens(char* command);
void piping(char* command);
void printprompt();
void getflag(int x);
void getcd(int y);
void getecho(int y);
void getquit();
void getpwd();
void getls(char* dir_name,int x,int y);
void getlsflags(int y);
void getpermission(char* filepath);
void foreground();
void getpinfo(int y);
void getrepeat(int y);
void exit_child();
void background(int y);
void getjobs(int y);
int compare(const void* p,const void* q);
void getfg(int y);
void getbg(int y);
void getsig(int y);
void ctrlC(int signum);
void ctrlZ(int signum);
void remove_job(int id);
int tokenize(char* command);
void execute_redirection(int x,int input,int output);