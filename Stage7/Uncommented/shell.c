#include "shell.h"
#include "commands.c"
#include "history.c"
#include "alias.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define Input_Max 512
#define Delimiter " \n\t;&><|"

char* InitialHomeEnv;
char* InitialPathEnv;

char* current_history[999];
char* command_aliases[10][2];

char* internal_commands[] = {
  "getpath",
  "setpath",
  "cd",
  "history",
  "alias",
  "unalias"
};

int (*internal_functions[]) (char **) = {
  &get_path,
  &set_path,
  &cd,
  &print_history,
  &add_alias,
  &unalias
};

int main(void)
{
	startup_initialize();

	char* line;
	char* token;
	char** tokens;
	char temp[Input_Max];

	while(1) {
		line = get_user_input();

		if(line[0] != '!')
		{
			sprintf(temp, "%s", line);
			add_to_history(temp);	
		}

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
	printf("> ");

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

		if(pos >= buffer) {
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

		token = strtok(NULL, Delimiter);
	}

	if(tokens[pos] != NULL)
		tokens[pos] = NULL;

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

	if (chdir(getenv("HOME")) != 0) {
		perror("Directory change failed");
	}

	if(load_history() == -1)
		printf("Failed to load history\n");
}

void exit_program(void)
{
	if(setenv("HOME", InitialHomeEnv, 1) == -1)
		perror("Environment reset failed");

	if(setenv("PATH", InitialPathEnv, 1) == -1)
		perror("Environment reset failed");

	if(chdir(getenv("HOME")) != 0)
		perror("Directory change failed");

	if(save_history() == -1)
		printf("Failed to save history.");

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

