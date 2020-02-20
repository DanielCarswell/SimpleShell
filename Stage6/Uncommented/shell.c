#include "shell.h"
#include "commands.c"
#include "history.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define Input_Max 512
#define Delimiter " \n\t;&><|"

char* InitialHomeEnv;
char* InitialPathEnv;

char* current_history[500];

char* internal_commands[] = {
  "getpath",
  "setpath",
  "cd",
  "history"
};

int (*internal_functions[]) (char **) = {
  &getpath,
  &setpath,
  &cd,
  &print_history
};

int main(void)
{
	InitialHomeEnv = (char*) malloc(100*sizeof(char*));
	InitialPathEnv = (char*) malloc(100*sizeof(char*));

	strcpy(InitialHomeEnv,getenv("HOME"));
	strcpy(InitialPathEnv,getenv("PATH"));

	if (chdir(getenv("HOME")) != 0) {
		perror("Directory change failed");
	}

	if(load_history() == -1)
		printf("Failed to load history");

	char* line;
	char* token;
	char** tokens;
	char temp[Input_Max];
	int exit = 0;

	signal(SIGTSTP, ctrlzIgnore);
	signal(SIGINT, ctrlcIgnore);

	do {

		line = getUserInput();
		
		if(line[0] != '!')
			{
				sprintf(temp, line);
				exit = add_to_history(temp);	
			}

		//Tokenize and parse user input.
		token = strtok(line, Delimiter);

		if (token) {
			tokens = parseInput(token);

			exit = runProcess(tokens);
		
			int pos = 0;
			while(tokens[pos] == NULL)
			{
				free(tokens[pos]);
				pos++;	
			}

			free(tokens);
			tokens = NULL;
  		}
		
	} while(exit != -1);

	resetPaths();
	return exit;
}

char* getUserInput(void)
{
	char *line = (char *) malloc(sizeof(char) * Input_Max);

	printf("> ");
	if(fgets(line, Input_Max, stdin) == NULL)
	{
		printf("\n");		
		resetPaths();
		_exit(1);
	}

	return line;
}

char** parseInput(char* token)
{
	int pos = 0;
	int buffer = 100;

	char** tokens = malloc(buffer * sizeof(char*));

	if(strcmp(token, "exit") == 0)
	{
		printf("\n");
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

		token = strtok(NULL, Delimiter);
	}

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

	if(save_history() == -1)
		printf("Failed to save history.");

	int pos = 0;
	while(current_history[pos] == NULL)
	{
		free(current_history[pos]);
		pos++;	
	}

	free(InitialHomeEnv);
	free(InitialPathEnv);
}

int internalCommandsCount(void)
{
	int size = 0;
	while(internal_commands[size] != NULL)
	{
		size++;
	}

	return size;
}

void ctrlzIgnore(int sig_num) 
{ 
    signal(SIGTSTP, ctrlzIgnore);
}

void ctrlcIgnore(int sig_num) 
{ 
	signal(SIGINT, ctrlcIgnore);
}
