#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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