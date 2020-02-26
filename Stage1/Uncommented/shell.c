#include "shell.h"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define Input_Max 512

int main(void)
{
	char line[Input_Max];
	char* cmdToken;
	int exit = 0;

	signal(SIGTSTP, ctrlzIgnore);
	signal(SIGINT, ctrlcIgnore);

	do {
		printf("> ");

		if(fgets(line, Input_Max, stdin) == NULL)
		{
			printf("\n");
			return -1;
		}

		cmdToken = strtok(line, " \n\t;&><|");
		exit = printTokens(cmdToken);

	} while(exit != -1);

	return exit;
}

void ctrlzIgnore(int sig_num) 
{
	signal(SIGTSTP, ctrlzIgnore);
}

void ctrlcIgnore(int sig_num) 
{
	signal(SIGINT, ctrlcIgnore);
} 

