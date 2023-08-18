#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX 1024
#define ARGC 10

int split(char *commandstr,char *argv[])
{
    assert(commandstr);
    assert(argv);
    argv[0]=strtok(commandstr," ");
    if(argv[0]==NULL) return -1;
    int i=1;
    while(1)
    {
        argv[i]=strtok(NULL," ");
        if(argv[i]==NULL) break;
        i++;
    }
    return 0;
}

void debugprint(char *argv[])
{
    for(int i=0;argv[i];++i)
    {
        printf("%d:%s\n",i,argv[i]);
    }
}

void ShowEnv()
{
    extern char **environ;
    for(int i = 0; environ[i]; i++) printf("%d:%s\n", i, environ[i]);
}

int main()
{
    int last_exit=0;
    char myenv[32][256];
    int env_index=0;
    while(1)
    {
        char commandstr[MAX]={0};
        char *argv[ARGC]={0};
        printf("[liubang@VM-8-6-centos myproc]$ ");
        fflush(stdout);
        char *s = fgets(commandstr,sizeof(commandstr),stdin);
        assert(s);
        (void)s;
        commandstr[strlen(commandstr)-1]='\0';
        
        //对输入的命令进行切割
        int n = split(commandstr,argv);
        if(n!=0) continue;
        //debugprint(argv);

        if(strcmp(argv[0],"cd")==0)
        {
            if(argv[1] != NULL) chdir(argv[1]);
            continue;
        }
        else if(strcmp(argv[0],"export")==0)
        {
            if(argv[1]!=NULL)
            {
                strcpy(myenv[env_index],argv[1]);
                putenv(myenv[env_index++]);
            }
            continue;
        }
        else if(strcmp(argv[0],"env")==0)
        {
            //该函数的目的是查看父进程bash的环境变量列表
            ShowEnv();
            continue;
        }
        else if(strcmp(argv[0],"echo")==0)
        {
            const char *target_env = NULL;
            if(argv[1][0] == '$')
            {
                if(argv[1][1]=='?')
                {
                    printf("%d\n", last_exit);
                    continue;
                }
                else
                {
                    target_env = getenv(argv[1]+1);
                }
                if(target_env!=NULL) printf("%s=%s\n",argv[1]+1,target_env);
            }
            continue;
        }

        if(strcmp(argv[0],"ls")==0)
        {
            int pos=0;
            while(argv[pos]) pos++;
            argv[pos]="--color=auto";
            argv[++pos]=NULL;
        }
        int id=fork();
        assert(id>=0);
        (void)id;
        if(id==0)
        {
            execvp(argv[0],argv); 
            exit(0);
        }
        int status=0;
        pid_t ret = waitpid(id, &status, 0);
        if(ret > 0){
            last_exit = WEXITSTATUS(status);
        }
    }
    return 0;
}