/*
 * CS3600, Spring 2013
 * Project 1 Starter Code
 * (c) 2013 Alan Mislove
 *
 * You should use this (very simple) starter code as a basis for
 * building your shell.  Please see the project handout for more
 * details.
 */

#ifndef _3600sh_h
#define _3600sh_h

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>

void do_exit();
void getargs(char *cmd, int *argcp, char *argv[]);
char* getcmd(char* beginning, char** end_of_cmd, char *argv[], int *argcp);
void execute(char* childargv[], int* ampersand, pid_t *pidsInBackground[]);
void parse_argument_array(int *childargc, char* childargv[], int* ampersand, pid_t *pidsInBackground[]);
void print_prompt();
void move_string(char* startingLoc);

#endif 
