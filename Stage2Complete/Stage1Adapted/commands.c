#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//For stage 1 demonstration.
//Prints each token in the commands double char pointer.
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
