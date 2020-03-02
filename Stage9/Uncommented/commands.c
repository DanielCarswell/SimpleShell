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
extern void (*internal_functions[]) (char**);

void choose_process(char** commands)
{
	if(strncmp(commands[0], "!", 1) == 0)
	{
		int check = element_in_history(commands);
		if(check == 1)
			return;
	}

	int check = check_alias(commands);
	if(check == 1)
		return;

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

void cd(char** commands)
{
	char* temp = malloc(1);

	if(commands[1] != NULL)
		strcpy(temp, command_line(commands, 1));
	else
	{
		printf("No path change entered\n");
		return;
	}

	if(commands[2] != NULL)
		printf("cd only accepts one parameter\n");
	else if(strcmp(commands[1], "~") == 0)
	{
		if (chdir(getenv("HOME")) != 0)
			perror("Directory change to HOME failed");
	}
	else
		if (chdir(commands[1]) != 0)
			perror(temp);
	
    free(temp);
}

void get_path(char** commands)
{
	if(commands[1] == NULL)
		printf("%s\n", getenv("PATH"));
	else
		printf("No parameters allowed for getpath\n");
	
	return;
}

void set_path(char** commands)
{
	if(commands[1] == NULL)
	{
		printf("No path specified\n");
		return;
	}
	else if(commands[2] != NULL)
	{
		printf("Too many arguments, setpath only takes 1 parameter\n");
	}

	if(strcmp(commands[1], "~") == 0) {
		if(setenv("PATH", InitialPathEnv, 1) == -1)
			perror("Failed to reset environment PATH");
		return;
	}

	if(setenv("PATH", commands[1], 1) == -1)
		perror("Failed to change environment PATH");
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