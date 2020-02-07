#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//External global variables, initialised in shell.c.
extern char* InitialPathEnv;
extern char* current_history[];
extern char* internal_commands[];
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
		if(iuu == 1)
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

//Created in Stage 4, changes the current working directory
//to a different directory, moving between folders by inputting the name
//of a folder that exists in the current directory, .. to step out of a directory
//or an absolute path.
int cd(char** commands)
{
	//Checks if the 2nd part of commands is null, printing a message if so.
	if(commands[1] == NULL)
		printf("No path change entered\n");
	
	//in the case the command is not null.	
	else
		//Attempts to change current directory and sends string to perror if fails.
		if (chdir(commands[1]) != 0)
			perror("Directory change failed");

    //Returns 0, closing the function.    	
    return 0;
}

//Created in Stage 3, displays the 'PATH' variable data.
int getpath(char** commands)
{
	//Print String stored in 'PATH' variable.
	printf("%s\n", getenv("PATH"));

	//Returns 0, this method normally would not require a return
	//type however it is necessary to allow all inbuilt functions
	//to be ran from the one function pointer array.
	return 0;
}

//Created in Stage 3, changes 'PATH' variable depending on user input.
int setpath(char** commands)
{
	//If the 2nd part of command is empty.
	if(commands[1] == NULL)
	{
		//Prints that the user did not specify a path and returns 0.
		printf("No path specified\n");
		return 0;
	}

	//If the 2nd part of command entered was '~'.
	if(strcmp(commands[1], "~") == 0)
	{
		//Attempts to set environment 'PATH' variable to original value.
		int setEnv = setenv("PATH", InitialPathEnv, 1);

		//Send message to perror if fails.
		if(setEnv == -1)
			perror("Failed to reset environment PATH");

		//Return 0 exitting the function.
		return 0;
	}
	
	//Attempts to set the 'PATH' variable to
	int setEnv = setenv("PATH", commands[1], 1);

	//If changing 'PATH' variable failed, sends message to perror.
	if(setEnv == -1)
		perror("Failed to change environment PATH");

	//Returns 0, exitting function.
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
			run_history(1, commands);
		else
			for(int i = 0; i < 10; i++)
			{
				if(strncmp(&commands[0][1], chars[i], 1) == 0)
					if(strncmp(&commands[0][2], "\0", 1) == 0)
						check = run_history(i, commands);
					else
						for(int j = 0; j < 10; j++)
						{
							if(strncmp(&commands[0][2], chars[j], 1) == 0)
								if(strncmp(&commands[0][3], "\0", 1) == 0)
									check = run_history(((i*10) + j), commands);
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

int run_history(int i, char** commands)
{
	//Current error: No such file or directoy, rewriting commands having some problem?
	if(current_history[i-1] != NULL)
	{
		free(*commands);
		commands = NULL;
		printf("%d%s\n", i, current_history[i-1]);
		char* ab = strtok(strdup(current_history[i-1]), " \n");
		commands = parseInput(ab);
		printTokens(commands);
		free(ab);
		return 0;
	}
	else
		printf("No element in history\n");
	return 1;
}
