#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char* InitialPathEnv;
extern char* internal_commands[];
extern char* current_history[];
extern int (*internal_functions[]) (char**);

//For stage 1 demonstration.
//Prints out each token in commands after running
//strtok on the user input and passing into function
//this function returns an integer to determine if
//the shell should continue processing or exit.
int printTokens(char** commands)
{
	//Declare local variables.
	int pos = 0;

	//Loop whilst commands is not NULL.
	while(commands[pos] != NULL) {

		//Check if the user entered "exit"
		//If "exit" was entered, returns -1.
		if(strcmp(commands[pos], "exit") == 0) return -1;

		//Display each token enclosed by quotations on seperate lines.
		printf("\"%s\"\n", commands[pos]);

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
	if(strncmp(commands[0], "!", 1) == 0)
	{
		int iuu = element_in_history(commands);
		return 0;
	}
	//Handling internal commands.
	for(int i = 0; i<3;i++)
	{
		if(strcmp(commands[0], internal_commands[i]) == 0)
			return (*internal_functions[i])(commands);
	}

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

int element_in_history(char** commands)
{
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	//Uses strncmp for comparing an nth element, instead of strcmp
	//which compares a full string.
	if(strncmp(commands[0], "!", 1) == 0)
	{
		char* abc = commands[0];
		if(strncmp(&commands[0][1],"\0",1) == 0)
			printf("No element of history selected\n");
		else if(strncmp(&commands[0][1],"!",1) == 0)
			run_history(1);
		else
			for(int i = 0; i < 10; i++)
			{
				if(strncmp(&commands[0][1], chars[i], 1) == 0)
					if(strncmp(&commands[0][2], "\0", 1) == 0)
						check = run_history(i);
					else
						for(int j = 0; j < 10; j++)
						{
							if(strncmp(&commands[0][2], chars[j], 1) == 0)
								if(strncmp(&commands[0][3], "\0", 1) == 0)
									check = run_history((i*10) + j);
								else
									printf("That element of history is out of bounds\n");
						}
				else if(i == 9 && check == 0 && strncmp(&commands[0][3], "\0", 1) == 0)
				{
					printf("Invalid character after !\n");
					break;
				}
			}
	}

	return check;
}

int run_history(int i)
{
	printf("%d%s\n", i, current_history[0]);
	char* token = strtok(current_history[0], " \n");
	char** oldCommand = parseInput(token);
	int status = runProcess(oldCommand);
	free(*oldCommand);
	oldCommand = NULL;
	return 1;
}

int cd(char** commands)
{
	if(commands[1] == NULL)
		printf("No path change entered\n");
	else
		if (chdir(commands[1]) != 0)
			perror("Directory change failed");
    	
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
	//returns -1 on error.
	int setEnv = setenv("PATH", commands[1], 1);

	if(setEnv == -1)
		perror("Failed to change environment PATH");
	//printf("\n%s\n", getenv("PATH"));

	return 0;
}