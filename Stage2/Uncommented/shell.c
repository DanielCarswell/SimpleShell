#include "shell.h"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define Input_Max 512
#define Delimiter " \n\t;&><|"

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
			run_process(tokens);
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
	{
		printf("\n");
		_exit(1);
	}

	return line;
}

char** parse_input(char* token)
{
	int pos = 0;
	int buffer = 100;

	char** tokens = malloc(buffer * sizeof(char*));

	if(strcmp(token, "exit") == 0)
	{
		printf("\n");
		_exit(1);
	}

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

	if(tokens[pos] != NULL)
		tokens[pos] = NULL;

	return tokens;
}

void startup_initialize(void)
{
	signal(SIGTSTP, ctrlz_ignore);
	signal(SIGINT, ctrlc_ignore);
}

void ctrlz_ignore(int sig_num) 
{ 
	signal(SIGTSTP, ctrlz_ignore);
}

void ctrlc_ignore(int sig_num) 
{
	signal(SIGINT, ctrlc_ignore);
} 

