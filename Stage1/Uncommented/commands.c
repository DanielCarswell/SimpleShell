#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int printTokens(char* cmdToken)
{
	while(cmdToken) {
		if(strcmp(cmdToken, "exit") == 0) return -1;
		
		printf("\"%s\"\n", cmdToken);
		cmdToken = strtok(NULL, " \n\t;&><|");
	}

	return 0;
}
