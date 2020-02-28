#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define Input_Max 512

extern char* InitialPathEnv;
extern char* current_history[];
extern char* internal_commands[];
extern int (*internal_functions[]) (char**);

void choose_process(char** commands)
{
	for(int i = 0; i < internal_commands_count(); i++)
	{
		if(strcmp(commands[0], internal_commands[i]) == 0)
		{
			(*internal_functions[i])(commands);
			return;
		}
	}

	run_process(commands);
}

void run_process(char** commands)
{
	char* temp = malloc(1);
	pid_t c_pid, pid;
	int status;

	strcpy(temp, command_line(commands, 0));
	printf("\n");

	c_pid = fork();

	if(c_pid == -1) {
		perror("Fork Failed");
		_exit(1);
	}

	if(c_pid == 0) {
		execvp(commands[0], commands);
		perror(temp);
		_exit(1);
	} 

	else if (c_pid > 0) {
		
		if ((pid = wait(&status)) < 0) {
			perror("Wait Failed");
			_exit(1);
		}
	}

	free(temp);
}

int get_path(char** commands)
{
	printf("%s\n", getenv("PATH"));
	return 0;
}

int set_path(char** commands)
{
	if(commands[1] == NULL)
	{
		printf("No path specified\n");
		return 0;
	}
	else if(commands[2] != NULL)
	{
		printf("Too many arguments, setpath only takes 1 parameter\n");
	}

	if(strcmp(commands[1], "~") == 0) {
		if(setenv("PATH", InitialPathEnv, 1) == -1)
			perror("Failed to reset environment PATH");
		return 0;
	}

	if(setenv("PATH", commands[1], 1) == -1)
		perror("Failed to change environment PATH");

	return 0;
}

char* command_line(char** commands, int position)
{
	int pos = position;
	char* line = malloc(1);
	line = NULL;

	while(commands[pos] != NULL)
	{
		line = realloc(line, (Input_Max * sizeof(char)));
		if(pos == position)
			sprintf(line, "%s", commands[pos]);
		else
			sprintf(line, "%s %s", line, commands[pos]);
		pos++;
	}

	return line;
}