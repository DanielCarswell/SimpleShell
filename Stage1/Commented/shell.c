#include "shell.h"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define Input_Max 512


//Main component of the shell, cycles and continues to ask for commands
//Then interpret them and execute appropriate functions or methods.
int main(void)
{
	//Initialise local variables.
	char line[Input_Max];
	char* cmdToken;
	int exit = 0;

	//Initialising Ctrl-z and Ctrl-z signal ignore.
	signal(SIGTSTP, ctrlzIgnore);
	signal(SIGINT, ctrlcIgnore);

	//Loop until "exit" or ctrl-d entered by user.
	do {
		//Prompt user for input.
		printf("> ");

		//Read user input. 
		if(fgets(line, Input_Max, stdin) == NULL)
		{
			printf("\n");
			return -1;
		}

		//Parse user input.
		cmdToken = strtok(line, " \n\t;&><|");
		exit = printTokens(cmdToken);
	} while(exit != -1);

	return exit;
}

void ctrlzIgnore(int sig_num) 
{ 
	//Resets the signal to stop upon ctrl-z input again.
        signal(SIGTSTP, ctrlzIgnore);
}

void ctrlcIgnore(int sig_num) 
{ 
	//Resets the signal to stop upon ctrl-c input again.
	signal(SIGINT, ctrlcIgnore);
} 

