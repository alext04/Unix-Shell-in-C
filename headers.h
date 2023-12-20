#ifndef HEADERS_H_
#define HEADERS_H_


#define MAX_COMMAND_LENGTH 1024
// #define MAX_COMMAND_LENGTH 100
#define MAX_HISTORY_SIZE 15
#define MAX_HISTORY_SIZE_5 3
#define PATH_MAX 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include "prompt.h"
#include <time.h>
#include <sys/stat.h>
#include <grp.h>
#include <dirent.h>

#include<ftw.h>


struct node{
    int pid;
    char command[265];
    struct node * next;
};

typedef struct node * pidlist;



void get_username_systemname_and_directory(const char *);
void execute_command(char *);
int is_built_in_command(const char *);


void tokenize_input(char *, char **, const char *);
void process_commands(char * ,const char * , char *,pidlist);
void execute_external_command(const char *, int );

//3
void warp(char *,const char * , char *);


//4
void print_permissions(mode_t );
void peek_directory(const char *, int, int );




//5


void add_to_pastevents(char *);
void print_pastevents();
void purge_pastevents();
void past_execute(int , char *, const char *, char *, pidlist);

//6
void checkbg(pidlist);


// 8
// void search_recursive(const char *, const char *, int , int , int );
// int process_entry(const char *, const struct stat *, int , int , int );
int seek_callback(const char *, const struct stat *, int , struct FTW *);
void seek(const char *, const char *, int , int , int );





#endif