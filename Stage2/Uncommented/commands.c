#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define Input_Max 512

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
