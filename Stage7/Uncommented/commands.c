#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char* InitialPathEnv;
extern char* current_history[];
extern char* internal_commands[];
extern int (*internal_functions[]) (char**);

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
	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if(c_pid == -1) {
		perror("Fork Failed");
		_exit(1);
	}

	if(c_pid == 0) {
		execvp(commands[0], commands);
		perror("Exec Failed");
		_exit(1);
	} 

	else if (c_pid > 0) {
		
		if ((pid = wait(&status)) < 0) {
			perror("Wait Failed");
			_exit(1);
		}
	}
}

int cd(char** commands)
{
	if(commands[1] == NULL)
		printf("No path change entered\n");
	else if(strcmp(commands[1], "~") == 0)
	{
		if (chdir(getenv("HOME")) != 0)
			perror("Directory change failed");
	}
	else
		if (chdir(commands[1]) != 0)
			perror("Directory change failed");
	
    return 0;
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

	if(strcmp(commands[1], "~") == 0) {
		if(setenv("PATH", InitialPathEnv, 1) == -1)
			perror("Failed to reset environment PATH");

		return 0;
	}

	if(setenv("PATH", commands[1], 1) == -1)
		perror("Failed to change environment PATH");

	return 0;
}
