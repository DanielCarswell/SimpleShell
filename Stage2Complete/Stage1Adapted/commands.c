#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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