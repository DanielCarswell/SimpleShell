#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char* InitialPathEnv;
extern char* internal_commands[];
extern int (*internal_functions[]) (char**);

int printTokens(char** commands)
{
	int pos = 0;

	while(commands[pos] != NULL) {

		if(strcmp(commands[pos], "exit") == 0) return -1;

		printf("\"%s\"\n", commands[pos]);

		pos++;
	}

	return 0;
}

int runProcess(char** commands)
{
	for(int i = 0; i< internalCommandsCount(); i++)
	{
		if(strcmp(commands[0], internal_commands[i]) == 0)
			return (*internal_functions[i])(commands);
	}

	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if(c_pid == -1)
	{
		perror("Fork Failed");
		_exit(1);
	}
	if(c_pid == 0)
	{
		execvp(commands[0], commands);

		perror("Exec Failed");
		_exit(1);
	} else if (c_pid > 0) {
		if ((pid = wait(&status)) < 0) {

			perror("Wait Failed");
			_exit(1);
		}
	}

	return 0;
}

int getpath(char** commands)
{
	printf("%s\n", getenv("PATH"));

	return 0;
}

int setpath(char** commands)
{
	if(commands[1] == NULL)
	{
		printf("No path specified\n");
		return 0;
	}

	if(strcmp(commands[1], "~") == 0)
	{
		int setEnv = setenv("PATH", InitialPathEnv, 1);

		if(setEnv == -1)
			perror("Failed to reset environment PATH");

		return 0;
	}
	
	int setEnv = setenv("PATH", commands[1], 1);

	if(setEnv == -1)
		perror("Failed to change environment PATH");

	return 0;
}
