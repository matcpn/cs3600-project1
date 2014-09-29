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
#define MAXARGS 30

int main(int argc, char*argv[]) {
  // Code which sets stdout to be unbuffered
  // This is necessary for testing; do not change these lines
  USE(argc);
  USE(argv);
  setvbuf(stdout, NULL, _IONBF, 0); 
  
  char *cmd = calloc(MAX_LINE_SIZE, sizeof(char));
  char *childargv[MAXARGS];
  int childargc = 0;
  int ampersand = 0;

  pid_t *pidsInBackground;
  pidsInBackground = (pid_t *)calloc(1, sizeof(pid_t));
  pidsInBackground[0] = -1; // There are no pids, but 0 would've meant it was a child.


  // Main loop that reads a command and executes it
  while (1) {         
    // You should issue the prompt here
    print_prompt();    
 
    // You should read in the command and execute it here
    while (!feof(stdin) && fgets(cmd, MAX_LINE_SIZE, stdin) != NULL) {
        getargs(cmd, &childargc, childargv);
        for (int i = 0; i < MAX_LINE_SIZE; i++) {
        	*(cmd + i) = '\0';
        }
    }
    parse_argument_array(&childargc, childargv, &ampersand, &pidsInBackground);

do_exit();
	  // Wait for all the background process pids
	  int pidPlace = 0;
	  for( ; pidsInBackground[pidPlace] != -1; pidPlace++) {
	    waitpid(pidsInBackground[pidPlace], 0, 0);
	  }
	  free(pidsInBackground);
    
      exit(0);
  }
  
  return 0;
}

// Function which exits, printing the necessary message
//
void do_exit() {
  
  printf("So long and thanks for all the fish!\n");
}

void print_prompt() {
  char dir[PATH_MAX];
  char host[MAX_HOST_CHARS];
  gethostname(host, MAX_HOST_CHARS);
  getcwd(dir, 1024);

  printf("%s@%s:%s> ", getenv("USER"), host, dir);
}

void getargs(char *cmd, int *argcp, char *argv[])
{
    char* end;

    //reads from standard in

    //parse the string into flags
    while ( (cmd = getcmd(cmd, &end, argv, argcp)) != NULL ) {
       argv[*argcp] = calloc(100, sizeof(char));
       strcpy(argv[*argcp],cmd);
       (*argcp)++;
       cmd = end + 1; // get past the '\0'
    }
  argv[*argcp] = NULL; //put a null after the last argument
}

char* getcmd(char* beginning, char** end_of_cmd, char *argv[], int *argcp) 
{
  char* end = beginning; //make a new pointer to show where the end will be
  while (*beginning == ' ')
    beginning++; //get rid of spaces
  while ( *end != '\0' && *end != ' ' ) {
  	if (*end == '\\') {
  		//this will get rid of slash and move everything to fix it
  		move_string(end);
  		if (*end == 't') {
  			*end = '\t';
  		}
  		else if (*end == ' ') {

  		}
  		else if (*end == '&') {

  		}
  		else if (*end == '\\') {

  		}
  		else {
  			printf("Error: Unrecognized escape sequence.");
  			argv[0] = NULL;
  			return "";
  		}
  	}
    if (*end == '\n') {
      *end = '\0';
      argv[*argcp] = calloc(100, sizeof(char));
      strcpy(argv[*argcp], beginning);
      (*argcp)++;
      argv[*argcp] = "\n";
      (*argcp)++;
      return NULL;
    }
    end++; // find the end of the command (either a space, null, or newline)

  }
  if (end == beginning)
    return NULL; //all words parsed 
  *end = '\0'; //put a null terminator after the command
  *end_of_cmd = end;
  return beginning; //begin is now the argument/flag without any spaces
}

void move_string(char* startingLoc) 
{
	char* cmd = startingLoc;
	while (*cmd != '\0' && *cmd != '\n') {
		*cmd = *(cmd + 1);
		cmd++;
	}
	*cmd = '\0'; 
}

void execute(char* childargv[], int* ampersand, pid_t *pidsInBackground[]) 
{
  pid_t p_id = fork();
    if (p_id == -1) {
      printf(" (fork failed)\n");
    }
    else if (p_id == 0) {
      if (strcmp(childargv[0], "exit") == 0) {
         do_exit();
      }
      else if (-1 == execvp(childargv[0], childargv)) {
        printf("Error: Command not found.\n");
        _Exit(EXIT_FAILURE);
      }
    }
    else {
    	if (*ampersand) {
    			//find where the last pids in the list is
      		int i = 0;
      		for ( ; (*pidsInBackground)[i] != -1; i++) {
      			//just loop and set i to be the last element
      		}
      		pid_t *temp = *pidsInBackground;
			*pidsInBackground = (pid_t *)calloc(i+2, sizeof(pid_t));
			(*pidsInBackground)[0] = p_id;
			for (i = 0; temp[i] != -1; i++) {
				(*pidsInBackground)[i+1] = temp[i];
			}
			(*pidsInBackground)[i+1] = -1;

			free(temp);
    	}
    	else {
   			waitpid(p_id, NULL, 0);
  		}
    } 
  return;
}


void execute_with_output_redir(char* childargv[], char* file, int* ampersand, pid_t *pidsInBackground[]) 
{
  pid_t p_id = fork();
    if (p_id == -1) {
      printf(" (fork failed)\n");
    }
    else if (p_id == 0) {
      if (strcmp(childargv[0], "exit") == 0) {
         do_exit();
      }
      freopen(file, "w", stdout);
      if (-1 == execvp(childargv[0], childargv)) {
        printf("Error: Command not found.\n");
        _Exit(EXIT_FAILURE);
      }
    }
    else {
      if (*ampersand) {
      		//find where the last pids in the list is
      		int i = 0;
      		for ( ; (*pidsInBackground)[i] != -1; i++) {
      			//just loop and set i to be the last element
      		}
      		pid_t *temp = *pidsInBackground;
			*pidsInBackground = (pid_t *)calloc(i+2, sizeof(pid_t));
			(*pidsInBackground)[0] = p_id;
			for (i = 0; temp[i] != -1; i++) {
				(*pidsInBackground)[i+1] = temp[i];
			}
			(*pidsInBackground)[i+1] = -1;

			free(temp);
    	}
    	else {
   			waitpid(p_id, NULL, 0);
  		}
    } 
  return;
}

void execute_with_input_redir(char* childargv[], char* file, int* ampersand, pid_t *pidsInBackground[]) 
{
  pid_t p_id = fork();
    if (p_id == -1) {
      printf(" (fork failed)\n");
    }
    else if (p_id == 0) {
      if (strcmp(childargv[0], "exit") == 0) {
         do_exit();
      }
      freopen(file, "r", stdin);
      if (-1 == execvp(childargv[0], childargv)) {
        printf("Error: Command not found.\n");
        _Exit(EXIT_FAILURE);
      }
    }
    else {
      if (*ampersand) {
      		//find where the last pids in the list is
      		int i = 0;
      		for ( ; (*pidsInBackground)[i] != -1; i++) {
      			//just loop and set i to be the last element
      		}
      		pid_t *temp = *pidsInBackground;
			*pidsInBackground = (pid_t *)calloc(i+2, sizeof(pid_t));
			(*pidsInBackground)[0] = p_id;
			for (i = 0; temp[i] != -1; i++) {
				(*pidsInBackground)[i+1] = temp[i];
			}
			(*pidsInBackground)[i+1] = -1;

			free(temp);
    	}
    	else {
   			waitpid(p_id, NULL, 0);
  		}
    } 
  return;
}

void execute_with_error_redir(char* childargv[], char* file, int* ampersand, pid_t *pidsInBackground[]) 
{
  pid_t p_id = fork();
    if (p_id == -1) {
      printf(" (fork failed)\n");
    }
    else if (p_id == 0) {
      if (strcmp(childargv[0], "exit") == 0) {
         do_exit();
      }
      freopen(file, "w", stderr);
      if (-1 == execvp(childargv[0], childargv)) {
        printf("Error: Command not found.\n");
        _Exit(EXIT_FAILURE);
      }
    }
    else {
      if (*ampersand) {
      		//find where the last pids in the list is
      		int i = 0;
      		for ( ; (*pidsInBackground)[i] != -1; i++) {
      			//just loop and set i to be the last element
      		}
      		pid_t *temp = *pidsInBackground;
			*pidsInBackground = (pid_t *)calloc(i+2, sizeof(pid_t));
			(*pidsInBackground)[0] = p_id;
			for (i = 0; temp[i] != -1; i++) {
				(*pidsInBackground)[i+1] = temp[i];
			}
			(*pidsInBackground)[i+1] = -1;

			free(temp);
    	}
    	else {
   			waitpid(p_id, NULL, 0);
  		}
    } 
  return;
}

void execute_with_input_and_output_redir(char* childargv[], char* filein, char* fileout, int* ampersand, pid_t *pidsInBackground[]) 
{
  pid_t p_id = fork();
    if (p_id == -1) {
      printf(" (fork failed)\n");
    }
    else if (p_id == 0) {
      if (strcmp(childargv[0], "exit") == 0) {
         do_exit();
      }
      freopen(filein, "r", stdin);
      freopen(fileout, "w", stdout);
      if (-1 == execvp(childargv[0], childargv)) {
        printf("Error: Command not found.\n");
        _Exit(EXIT_FAILURE);
      }
    }
    else {
      if (*ampersand) {
      		//find where the last pids in the list is
      		int i = 0;
      		for ( ; (*pidsInBackground)[i] != -1; i++) {
      			//just loop and set i to be the last element
      		}
      		pid_t *temp = *pidsInBackground;
			*pidsInBackground = (pid_t *)calloc(i+2, sizeof(pid_t));
			(*pidsInBackground)[0] = p_id;
			for (i = 0; temp[i] != -1; i++) {
				(*pidsInBackground)[i+1] = temp[i];
			}
			(*pidsInBackground)[i+1] = -1;

			free(temp);
    	}
    	else {
   			waitpid(p_id, NULL, 0);
  		}
    } 
  return;
}

void check_for_valid_ampersand(char* childargv[], int* ampersand) {
	int i = 0;
	//go through ever element in the array
	while (childargv[i] != NULL) {
		//if an element is an ampersand
		if (strcmp(childargv[i], "&") == 0) {
			//check if it is the last element 
			if (childargv[i + 1] == NULL) {
				//if it is the last ampersand, we have a valid ampersand and need to run it in the background
				*ampersand = 1;
				childargv[i] = NULL;
				return;
			}
			else {
				//if it it not a valid ampersand. we print the error
				printf("Error: Invalid Syntax.");
				*ampersand = 0;
				return;
			}
		}
		//if and element is not an ampersand, does it contain an ampersand?
		else {
			char* currentLetter = childargv[i];
			while (*currentLetter != '\0' && *currentLetter != '\n') {
				//if current letter is an ampersand
				if (*currentLetter == '&')
				{
					//if current letter is the last in the argument
					if (*(currentLetter + 1) == '\0') {
						*ampersand = 1;
						*currentLetter = '\0';
						return;
					}
				}
				currentLetter++;
			}
		}
		i++;
	}
	*ampersand = 0;
	return;
}

void parse_argument_array(int *childargc, char* childargv[], int* ampersand, pid_t *pidsInBackground[]) 
{
  if (childargv[0] == NULL) {
  	printf("\n");
  	return;
  }
  char *temp_array[MAXARGS];
  for (int i = 0; i < MAXARGS; i++) {
    temp_array[i] = NULL;
  }
  int j = 0;
  for (int i = 0; i < *childargc; i++) {
    if (strcmp(childargv[i], "") == 0)
      continue;
    if (strcmp(childargv[i], "\n") == 0) {
      temp_array[j] = NULL;
      if (temp_array[0] != NULL) {
      	check_for_valid_ampersand(temp_array, ampersand);
        execute(temp_array, ampersand, pidsInBackground);
      }
      print_prompt();
      j=0;
    }
    else if (strcmp(childargv[i], "<") == 0) {
      if (strcmp(childargv[i+2], ">") == 0) {
      	temp_array[j] = NULL;
	      if (temp_array[0] != NULL) {
	      	check_for_valid_ampersand(temp_array, ampersand);
	        execute_with_input_and_output_redir(temp_array, childargv[i+1], childargv[i+3], ampersand, pidsInBackground);
	      }
	      j=0;
	      i++; 
      }
      else {
	      temp_array[j] = NULL;
	      if (temp_array[0] != NULL) {
	      	check_for_valid_ampersand(temp_array, ampersand);
	        execute_with_input_redir(temp_array, childargv[i+1], ampersand, pidsInBackground);
	      }
	      j=0;
	      i++; 
	  }
    }
    else if (strcmp(childargv[i], ">") == 0) {
      temp_array[j] = NULL;
      if (temp_array[0] != NULL) {
      	check_for_valid_ampersand(temp_array, ampersand);
        execute_with_output_redir(temp_array, childargv[i+1], ampersand, pidsInBackground);
      }
      j=0;
      i++; 
    }
    else if (strcmp(childargv[i], "2>") == 0) {
      temp_array[j] = NULL;
      if (temp_array[0] != NULL) {
      	check_for_valid_ampersand(temp_array, ampersand);
        execute_with_error_redir(temp_array, childargv[i+1], ampersand, pidsInBackground);
      }
      j=0;
      i++; 
    }
    else {
      temp_array[j] = childargv[i];
      j++;
    }
  }
  if (strcmp(childargv[*childargc - 1], "\n") != 0) {
  	check_for_valid_ampersand(temp_array, ampersand);
    execute(temp_array, ampersand, pidsInBackground);
  }
  return;
}