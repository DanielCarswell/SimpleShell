#include "shell.h"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define Input_Max 512
#define Delimiter " \n\t;&><|"

char* InitialHomeEnv;
char* InitialPathEnv;

char* internal_commands[] = {
  "getpath",
  "setpath"
};

void (*internal_functions[]) (char **) = {
  &get_path,
  &set_path
};

int main(void)
{
	startup_initialize();

	char* line;
	char* token;
	char** tokens;

	while(1) {
		line = get_user_input();
		token = strtok(line, Delimiter);
		
		if (token) {
			tokens = parse_input(token);
			choose_process(tokens);
			free(tokens);
  		}
	}

	return 0;
}

char* get_user_input(void)
{
	char *line = (char *) malloc(sizeof(char) * Input_Max);
	printf("\n> ");

	if(fgets(line, Input_Max, stdin) == NULL)
		exit_program();

	return line;
}

char** parse_input(char* token)
{
	int pos = 0;
	int buffer = 100;

	char** tokens = malloc(buffer * sizeof(char*));

	if(strcmp(token, "exit") == 0)
		exit_program();

	while(token)
	{
		tokens[pos] = token;
		pos++;

		if(pos>=buffer) {
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

		token = strtok(NULL, Delimiter);
	}

	while(tokens[pos] != NULL)
	{
		tokens[pos] = NULL;
		pos++;
	}

	return tokens;
}

void startup_initialize(void)
{
	signal(SIGTSTP, ctrlz_ignore);
	signal(SIGINT, ctrlc_ignore);

	InitialHomeEnv = (char*) malloc(sizeof(char) * strlen(getenv("HOME"))+1);
	InitialPathEnv = (char*) malloc(sizeof(char) * strlen(getenv("PATH"))+1);

	strcpy(InitialHomeEnv, getenv("HOME"));
	strcpy(InitialPathEnv, getenv("PATH"));

	if(setenv("PATH", InitialHomeEnv, 1) == -1)
		perror("Failed to change PATH environment");

	if (chdir(getenv("HOME")) != 0) {
		perror("Directory change failed");
	}
}

void exit_program(void)
{
	if(setenv("HOME", InitialHomeEnv, 1) == -1)
		perror("Environment reset failed.");

	if(setenv("PATH", InitialPathEnv, 1) == -1)
		perror("Environment reset failed.");

	free(InitialHomeEnv);
	free(InitialPathEnv);

	printf("\n");
	_exit(1);
}

int internal_commands_count(void)
{
	int size = 0;
	while(internal_commands[size] != NULL)
	{
		size++;
	}

	return size;
}

void ctrlz_ignore(int sig_num) 
{ 
	signal(SIGTSTP, ctrlz_ignore);
}

void ctrlc_ignore(int sig_num) 
{
	signal(SIGINT, ctrlc_ignore);
} 