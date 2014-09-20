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
#define MAX_LINE_SIZE 200 
#define MAXARGS 20

int main(int argc, char*argv[]) {
  // Code which sets stdout to be unbuffered
  // This is necessary for testing; do not change these lines
  USE(argc);
  USE(argv);
  setvbuf(stdout, NULL, _IONBF, 0); 
  
  char *cmd = calloc(MAX_LINE_SIZE, sizeof(char));
  char *childargv[MAXARGS];
  int childargc;

  // Main loop that reads a command and executes it
  while (1) {         
    // You should issue the prompt here
    char dir[PATH_MAX];
    char host[MAX_HOST_CHARS];
    gethostname(host, MAX_HOST_CHARS);
	getcwd(dir, 1024);

    printf("%s@%s:%s> ", getenv("USER"), host, dir);
    
 
    // You should read in the command and execute it here
    getargs(cmd, &childargc, childargv);
	execute(childargv);
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


void getargs(char *cmd, int *argcp, char *argv[])
{
    char* end;
    int i = 0;

    //reads from standard in
    if (fgets(cmd, MAX_LINE_SIZE, stdin) == NULL && feof(stdin)) {
        printf("Couldn't read from standard input. End of file? Exiting ...\n");
        exit(1); 
    }
    //parse the string into flags
    while ( (cmd = getcmd(cmd, &end)) != NULL ) {
       argv[i++] = cmd;
       cmd = end + 1; // get past the '\0'
    }
	argv[i++] = NULL; //put a null after the last argument
  *argcp = i;
}

char* getcmd(char* beginning, char** end_of_cmd) 
{
  char* end = beginning; //make a new pointer to show where the end will be
	while (*beginning == ' ')
    beginning++; //get rid of spaces
  while ( *end != '\0' && *end != '\n' && *end != ' ' )
        end++; // find the end of the command (either a space, null, or newline)
  if (end == beginning)
    return NULL; //all words parsed 
  *end = '\0'; //put a null terminator after the command
  *end_of_cmd = end;
  return beginning; //begin is now the argument/flag without any spaces
}

void execute(char* childargv[]) 
{
	pid_t p_id = fork();
	if (p_id == -1) {
		printf(" (fork failed)\n");
	}
    if (p_id == 0) {
		if (-1 == execvp(childargv[0], childargv)) {
			printf("Error: Command not found. %s\n", childargv[0]);
		}
	}
	else {
		waitpid(p_id, NULL, 0);
	}	
	return;
}
