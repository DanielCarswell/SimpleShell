#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//Main component of the shell, cycles and continues to ask for commands
//Then interpret them and execute appropriate functions or methods.
int shell_main(void)
{
	//Initialise local variables.
	char line[500];
	char* cmdToken;
	int exit = 0;

	do {
		//Prompt user for input.
		printf("> ");

		//Read user input.
		gets(line);

		//Parse user input.
		cmdToken = strtok(line, " ");
		while(cmdToken) {
			printf("\"%s\"\n", cmdToken);
			if(strcmp(cmdToken, "exit()") == 0)
			exit = -1;
			cmdToken = strtok(NULL, " ");
		}

	} while(exit != -1);

	return exit;
}