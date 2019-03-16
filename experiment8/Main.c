//
// Created by ricky on 19-3-1.
//

#include "Head.h"
#define TRUE 1
#define MAXPIDTABLE 1024

pid_t BPTable[MAXPIDTABLE];

void sig_handler()
{
    pid_t pid;
    int i;
    for(i=0;i<MAXPIDTABLE;i++)
        if(BPTable[i] != 0)
        {
            pid = waitpid(BPTable[i],NULL,WNOHANG);
            if(pid > 0)
            {
                printf("process %d exited.\n",pid);
                BPTable[i] = 0;
            }
            else if(pid < 0)
            {
                if(errno != ECHILD)
                    perror("waitpid error");
            }
        }
    return;
}

void proc(void)
{
    int status;
    char *command = NULL;
    char **parameters;
    int ParaNum;
    char prompt[MAX_PROMPT];
    struct parse_info info;
    pid_t ChdPid,ChdPid2;
    parameters = malloc(sizeof(char *)*(MAXARG+2));
    buffer = malloc(sizeof(char) * MAXLINE);
    if(parameters == NULL || buffer == NULL)
    {
        printf("shell error:malloc failed.\n");
        return;
    }

    if(signal(SIGCHLD,sig_handler) == SIG_ERR)
        perror("signal() error");

    while(TRUE)
    {
        int pipe_fd[2],in_fd,out_fd;
        Get_pwd(prompt);
        ParaNum = Get_comln(&command,parameters,prompt);
        if(-1 == ParaNum)
            continue;
        ParaNum--;
        parsing(parameters,ParaNum,&info);
        if(Run_command(command,parameters))
            continue;
        if(info.flag & IS_PIPED)
        {
            if(pipe(pipe_fd)<0)
            {
                printf("shell error:pipe failed.\n");
                exit(0);
            }
        }
        if((ChdPid = fork())!=0)
        {
            if(info.flag & IS_PIPED)
            {
                if((ChdPid2=fork()) == 0)
                {
                    close(pipe_fd[1]);
                    close(fileno(stdin));
                    dup2(pipe_fd[0], fileno(stdin));
                    close(pipe_fd[0]);
                    execvp(info.command2,info.parameters2);
                }
                else
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    waitpid(ChdPid2,&status,0);
                }
            }

            if(info.flag & BACKGROUND)
            {
                printf("Child pid:%u\n",ChdPid);
                int i;
                for(i=0;i<MAXPIDTABLE;i++)
                    if(BPTable[i]==0)
                        BPTable[i] = ChdPid;
                if(i==MAXPIDTABLE)
                    perror("Too much background processes\nThere will be zombine process");
            }
            else
            {
                waitpid(ChdPid,&status,0);
            }
        }
        else
        {

            if(info.flag & IS_PIPED)
            {
                if(!(info.flag & OUT_REDIRECT) && !(info.flag & OUT_REDIRECT_APPEND)) // ONLY PIPED
                {
                    close(pipe_fd[0]);
                    close(fileno(stdout));
                    dup2(pipe_fd[1], fileno(stdout));
                    close(pipe_fd[1]);
                }
                else
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    if(info.flag & OUT_REDIRECT)
                        out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    else
                        out_fd = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    close(fileno(stdout));
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
            }
            else
            {
                if(info.flag & OUT_REDIRECT)
                {
                    out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    close(fileno(stdout));
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
                if(info.flag & OUT_REDIRECT_APPEND)
                {
                    out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_APPEND, 0666);
                    close(fileno(stdout));
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
            }

            if(info.flag & IN_REDIRECT)
            {
                in_fd = open(info.in_file, O_CREAT |O_RDONLY, 0666);
                close(fileno(stdin));
                dup2(in_fd, fileno(stdin));
                close(in_fd);
            }
            execvp(command,parameters);
        }
    }
}

int main() {
    int i;
    for(i=0;i<MAXPIDTABLE;i++)
        BPTable[i] = 0;
    proc();
    return 0;
}
