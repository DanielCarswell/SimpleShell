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
int printTokens(char* cmdToken)
{
	//Loop whilst cmdToken is not NULL.
	while(cmdToken) {

		//Check if the user entered "exit"
		//If "exit" was entered, returns -1.
		if(strcmp(cmdToken, "exit") == 0) return -1;

		//Display each token enclosed by quotations on seperate lines.
		printf("\"%s\"\n", cmdToken);

	    //Move to next token.
		cmdToken = strtok(NULL, " \n\t;&><|");
	}

	//Returns 0, this will mean the shell will keep running.
	return 0;
}
