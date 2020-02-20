#include "shell.h"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char* InitialHomeEnv;
char* InitialPathEnv;

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

int cd(char** commands)

int main(void)
{
	InitialHomeEnv = (char*) malloc(100*sizeof(char*));
	InitialPathEnv = (char*) malloc(100*sizeof(char*));

	strcpy(InitialHomeEnv,getenv("HOME"));
	strcpy(InitialPathEnv,getenv("PATH"));

	int success = setenv("PATH", InitialHomeEnv, 1);

	if(success == -1)
		perror("Failed to change PATH environment");

	if (chdir(getenv("HOME")) != 0) {
		perror("Directory change failed");
	}

	char* line;
	char* token;
	char** tokens;
	int exit = 0;

	signal(SIGTSTP, ctrlzIgnore);
	signal(SIGINT, ctrlcIgnore);

	do {
		line = getUserInput();

		token = strtok(line, " \n");

		if (token) {
			tokens = parseInput(token);

			exit = runProcess(tokens);
		
			free(*tokens);
			tokens = NULL;
  		}
		
	} while(exit != -1);

	resetPaths();
	return exit;
}

char* getUserInput(void)
{
	char *line = (char *) malloc(sizeof(char) * 512);

	printf("> ");

	if(fgets(line, 512, stdin) == NULL)
	{
		resetPaths();
		_exit(1);
	}

	return line;
}

char** parseInput(char* token)
{
	//Declaring local variables.
	int pos = 0;
	int buffer = 100;

	char** tokens = malloc(buffer * sizeof(char*));

	if(strcmp(token, "exit") == 0)
	{
		resetPaths();
		_exit(1);
	}

	while(token != NULL)
	{
		tokens[pos] = token;
		pos++;

		if(pos>=buffer) {
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

		token = strtok(NULL, " \n");
	}

	if(tokens[pos] != NULL)
		tokens[pos] == NULL;

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
