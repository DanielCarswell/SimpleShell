#include "shell.h"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char* InitialHomeEnv;
char* InitialPathEnv;

char* current_history[] = {
	"ls -l"
};

char* internal_commands[] = {
  "getpath",
  "setpath",
  "cd"
};

int (*internal_functions[]) (char **) = {
  &getpath,
  &setpath,
  &cd
};

//Main component of the shell, cycles and continues to ask for commands
//Then interpret them and execute appropriate functions or methods.
int main(void)
{
	InitialHomeEnv = (char*) malloc(100*sizeof(char*));
	InitialPathEnv = (char*) malloc(100*sizeof(char*));

	strcpy(InitialHomeEnv,getenv("HOME"));
	strcpy(InitialPathEnv,getenv("PATH"));

	if (chdir(getenv("HOME")) != 0) {
      perror("Directory change failed");
    }

	//Initialise local variables.
	char* line;
	char* token;
	char** tokens;
	int exit = 0;

	//Initialising Ctrl-z and Ctrl-z signal ignore.
	signal(SIGTSTP, ctrlzIgnore);
	signal(SIGINT, ctrlcIgnore);

	//printf("%s", InitialPathEnv);
	//Loop until "exit" or ctrl-d entered by user.
	do {

		//Gets user to input commands.
		line = getUserInput();

		//Tokenize and parse user input.
		token = strtok(line, " \n");

		if (token) {
			tokens = parseInput(token);

			//Run command and return int.
			exit = runProcess(tokens);
		
			//Frees the current memory of tokens.
			free(*tokens);
			tokens = NULL;
  		}
		
	} while(exit != -1);

	resetPaths();
	//Returns exit to close shell since returning from main
	//successfully closes the program.
	return exit;
}

//Problem was occuring where I believe line was
//not being overwritten when asking for user input in main
//by creating a seperate method that returns a new line each
//time this problem was resolved.
char* getUserInput(void)
{
	//Declare local variable and allocate memory.
	char *line = (char *) malloc(sizeof(char) * 512);

	//Prompt user for input.
	printf("> ");

	//Read user input. 
	if(fgets(line, 512, stdin) == NULL)
	{
		resetPaths();
		//Exit shell.
		_exit(1);
	}

	//Return user input
	return line;
}

//In stage 1, I just used the strok Char* token for display
//For stage 2 to work properly I had to parse the data to
//a double pointer for the execvp command to interpret and
//run the command properly.
char** parseInput(char* token)
{
	//Declaring local variables.
	int pos = 0;
	int buffer = 100;

	//Allocates memories to tokens equal to buffer times size of char pointer.
	char** tokens = malloc(buffer * sizeof(char*));

	//Checks if user has inputted "exit" to close the shell.
	if(strcmp(token, "exit") == 0)
	{
		resetPaths();
		_exit(1);
	}

	//Loops whilst token is not NULL, so when another token exists.
	while(token != NULL)
	{
		//Set position(pos) of tokens equal to current token and increment pos.
		tokens[pos] = token;
		pos++;

		//Whilst the position(pos) is greater than or equal
		//to the size of buffer, increase buffer and reallocate
		//memory to tokens.
		if(pos>=buffer) {
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

		//Gets next token.
		token = strtok(NULL, " \n");
	}

	//Return tokens.
	return tokens;
}

void resetPaths(void)
{
	int envReset = setenv("HOME", InitialHomeEnv, 1);

	if(envReset == -1)
		perror("Environment reset failed.");

	envReset = setenv("PATH", InitialPathEnv, 1);

	if(envReset == -1)
		perror("Environment reset failed.");

	free(InitialHomeEnv);
	free(InitialPathEnv);
}

void ctrlzIgnore(int sig_num) 
{ 
	//Resets the signal to stop upon ctrl-z input again.
        signal(SIGTSTP, ctrlzIgnore);
}

void ctrlcIgnore(int sig_num) 
{ 
	//Resets the signal to stop upon ctrl-c input again.
	signal(SIGINT, ctrlcIgnore);
}