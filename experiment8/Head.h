//
// Created by ricky on 19-3-1.
//

#ifndef EXPERIMENT8_WSHELL_H
#define EXPERIMENT8_WSHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_PROMPT 1024
#define MAXLINE 4096
#define MAXARG 20

struct parse_info;
struct passwd *pwd;
char *buffer;

void Get_pwd(char*);
int Get_comln(char **,char **,char*);
int Run_command(char *,char **);
int parsing(char **,int,struct parse_info *);
void proc(void);
void sig_handler();



#ifndef STRUCT_PARSE_INFO
#define STRUCT_PARSE_INFO
#define BACKGROUND 			1
#define IN_REDIRECT 		2
#define OUT_REDIRECT 		4
#define OUT_REDIRECT_APPEND	8
#define IS_PIPED 			16
struct parse_info
{
    int flag;
    char* in_file;
    char* out_file;
    char* command2;
    char** parameters2;
};
#endif


#endif
