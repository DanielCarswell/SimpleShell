#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//For stage 1 demonstration.
//Prints out each token in cmdToken after running
//strtok on the user input and passing into function
//this function returns an integer to determine if
//the shell should continue processing or exit.
int printTokens(char** cmdToken)
{
	//Declare local variables.
	int pos = 0;

	//Loop whilst cmdToken is not NULL.
	while(cmdToken[pos] != NULL) {

		//Check if the user entered "exit"
		//If "exit" was entered, returns -1.
		if(strcmp(cmdToken[pos], "exit") == 0) return -1;

		//Display each token enclosed by quotations on seperate lines.
		printf("\"%s\"\n", cmdToken[pos]);

		//Increment position.
		pos++;
	}

	//Returns 0, this will mean the shell will keep running.
	return 0;
}

//For stage 2,
//This method was a remake of the fork() near end of 
//lecture 13 for semester 1 where it uses execvp instead of execv.
//Takes in a double pointer to char of tokens
//the tokens specify commands for the function
//to execute in a somewhat controlled environment
//thanks to forking and allowing the commands to run
//on the child process, since this has no risk that
//allowing it to run on the parent process could cause.
int runProcess(char** commands)
{
	//Declaring local variables.
	pid_t c_pid, pid;
	int status;

	//Beginning fork, splitting into child and parent processes.
	c_pid = fork();

	//If fork fails.
	if(c_pid == -1)
	{
		//Prints descriptive error message to stderr.
		perror("Fork Failed");
		_exit(1);
	}
	//Execute child process if c_pid is equal to 0.
	if(c_pid == 0)
	{
		//Execute command entered by user.
		execvp(commands[0], commands);

		//Prints descriptive error message to stderr.
		perror("Exec Failed");
		_exit(1);
	} else if (c_pid > 0) {
		if ((pid = wait(&status)) < 0) {
			//Prints descriptive error message to stderr.
			perror("Wait Failed");
			_exit(1);
		}
	}

	//Returns 0, this will mean the shell will keep running.
	return 0;
} 