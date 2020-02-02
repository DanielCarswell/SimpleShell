#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


//Main component of the shell, cycles and continues to ask for commands
//Then interpret them and execute appropriate functions or methods.
int main(void)
{
	//Initialise local variables.
	char line[512];
	char** tokens;
	char* token;
	int exit = 0;

	//Initialising Ctrl-z and Ctrl-z signal ignore.
	signal(SIGTSTP, ctrlzIgnore);
	signal(SIGINT, ctrlcIgnore);

	//Loop until "exit" or ctrl-d entered by user.
	do {
		//Prompt user for input.
		printf("> ");

		//Read user input. 
		if(fgets(line, 512, stdin) == NULL)
		{
			printf("\nexitting\n");
			_exit(1);
		}

		//Parse user input.
		token = strtok(line, " \n");
		tokens = parseInput(token);

		//Run command and return int.
		exit = runProcess(tokens);
		
		//Frees the current memory of tokens.
		free(tokens);
	} while(exit != -1);

	//Returns exit to close shell since returning from main
	//successfully closes the program.
	return exit;
}

//In stage 1, I just used the strok Char* token
char** parseInput(char* token)
{
	//Declaring local variables.
	int pos = 0;
	int buffer = 100;

	//Allocates memories to tokens equal to buffer times size of char pointer.
	char** tokens = malloc(buffer * sizeof(char*));

	//Checks if user has inputted "exit" to close the shell.
	if(strcmp(token, "exit") == 0)
			_exit(1);

	//Loops whilst token is not NULL, so when another token exists.
	while(token != NULL)
	{
		//Set position(pos) of tokens equal to current token and increment pos.
		tokens[pos] = token;
		pos++;

		//Whilst the position(pos) is greater than or equal
		//to the size of buffer, increase buffer and reallocate
		//memory to tokens.
		if(pos>=buffer) {
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

		//Gets next token.
		token = strtok(NULL, " \n");
	}

	//Sometimes an extra position of token is assigned
	//the last token of the last users input, this checks
	//if it has occured and sets it to NULL in such case. 
	if(tokens[pos] != NULL)
		tokens[pos] == NULL;

	//Return tokens.
	return tokens;
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

