/**
 * CS3600, Spring 2013
 * Project 1 Starter Code
 * (c) 2013 Alan Mislove
 *
 * You should use this (very simple) starter code as a basis for 
 * building your shell.  Please see the project handout for more
 * details.
 */

#include "3600sh.h"

#define USE(x) (x) = (x)
#define MAX_HOST_CHARS 64
#define MAXLINE 200 
#define MAXARGS 20

int main(int argc, char*argv[]) {
  // Code which sets stdout to be unbuffered
  // This is necessary for testing; do not change these lines
  USE(argc);
  USE(argv);
  setvbuf(stdout, NULL, _IONBF, 0); 
  char cmd[MAXLINE];
  char *childargv[MAXARGS];
  int childargc;

  // Main loop that reads a command and executes it
  while (1) {         
    // You should issue the prompt here
    char* test = calloc(PATH_MAX, sizeof(char));
    char* host = calloc(MAX_HOST_CHARS, sizeof(char));
    gethostname(host, MAX_HOST_CHARS);
	getcwd(test, 1024);

    printf("%s@%s:%s> ", getenv("USER"), host, test);
    
    free(test);
    free(host);  
    // You should read in the command and execute it here
    getargs(cmd, &childargc, childargv);

    // You should probably remove this; right now, it
    // just exits
    //do_exit();
  }

  return 0;
}

// Function which exits, printing the necessary message
//
void do_exit() {
  printf("So long and thanks for all the fish!\n");

  // Wait for all children to exit, then exit
  while (wait(NULL) > 0) {}
  exit(0);
}


static void getargs(char cmd[], int *argcp, char *argv[])
{
    char *cmdp = cmd;
    char *end;
    int i = 0;

    //reads from standard in
    if (fgets(cmd, MAXLINE, stdin) == NULL && feof(stdin)) {
        printf("Couldn't read from standard input. End of file? Exiting ...\n");
        exit(1); 
    }
    //parse the string into usable arguments
    while ((cmdp = getword(cmdp, &end)) != NULL ) {
	argv[i++] = cmdp;
	cmdp = end + 1;
    }
    argv[i] = NULL;
    *argcp = i;
}
