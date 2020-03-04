#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Definitions for reused values, 512 is input limit, and Delimiter is for tokenizing.
#define Input_Max 512
#define Delimiter " \n\t;&><|"

//Getting necessary external global variables for this module.
extern char* internal_commands[];
extern char* command_aliases[10][2];


//Called by choose_process method, used to check if a command entered is an alias or not
//before trying to run an alias as an external command.
int check_alias(char** commands)
{
	//Initializing local variable.
	int pos = 0;

	//Repeats while another alias exists.
	while(command_aliases[pos][0] != NULL)
	{
		//If the command token in pos 0, is found to be an alias.
		if(strcmp(command_aliases[pos][1], commands[0]) == 0)
		{
			//Check if that alias, is an alias of an alias and replace with the last alias in the cycle.
			recursive_check_alias(commands, command_aliases[pos][1]);

			//Call run alisa method, passing in commands and the aliased command to configure before running.
			run_alias(commands, strdup(command_aliases[pos][0]));

			//Return 1 as alias existed.
			return 1;
		}
		//increment pos.
		pos++;
	}

	//Return 0 as no alias found.
	return 0;
}


//Recursively checks alias passed, if that command exists in another alias as an alias
//This is for stage 9, though it works without this is just more efficient than the check_alias and run
//alias being  constantly invoked.
int recursive_check_alias(char** commands, char* alias)
{
	//Initializing local variable.
	int position = 0;

	//Repeats while another alias exists.
	while(command_aliases[position][1] != NULL)
	{
		//Checks if alias is a command in another alias command matrix pair.
		if(strcmp(alias, command_aliases[position][0]) == 0)
		{
			//Replaces token in position 0 of commands with alias found.
			strcpy(commands[0], command_aliases[position][1]);

			//Recursive call to check again if that aliases command is an alias elsewhere.
			recursive_check_alias(commands, command_aliases[position][1]);
		}

		//Increment position.
		position++;
	}

	//Returns 0 once no more aliases of command found.
	return 0;
}

/*
	Internal Command: alias <paramater> <one to many parameters>

	Takes in an alias name and a command after it, the alias and command are added to a matrix,
	or an alias by the same name is overwritten by the new command.
*/
void add_alias(char** commands)
{
	//This is to protect against random junk being added to .alias_list file being loaded in.
	if(strncmp(commands[0], "alias", 5) != 0)
	{
		//Takes a new line and returns if 'alias' is not first token in command.
		printf("\n");
		return;
	}

	//Initializing local variables.
	char* commandTemp;
	char* temp = (char*) malloc(sizeof(char)*Input_Max);
	int index = 0;

	//Cycles to make sure all parameters exist.
	for(int i = 0; i < 3; i ++)
	{
		//If position i of commands is null, then not enough parameters in command.
		if(commands[i] == NULL)
		{
			//If i is equal to 1, then this was a print alias invocation.
			if(i == 1)
				print_aliases();

			//otherwise, print appropriate message.
			else 
				printf("Not enough arguments\n");

			//Return to method call.
			return;
		}
	}

	//Get the char* value of the command for the alias.
	commandTemp = command_line(commands, 2);

	//Check if infinite loop creation was attempted.
	if(infinite_alias_check(commands[1], commandTemp))
	{
		//Prints appropriate message and returns.
		printf("This alias would cause an infinite loop and was not added.\n");
		return;
	}
	
	//Loops while another alias exists.
	while(command_aliases[index][0] != NULL)
	{
		//Checks if alias already exists and that a new command exists to add to alias.
		if(strcmp(command_aliases[index][1], commands[1]) == 0 && commandTemp != NULL)
		{
			//Prints message saying what the aliases command is being overwritten with.
			printf("Old alias overwritten, new command: %s\n", commandTemp);

			//Frees current memory allocation of pos 0 of pos index matrix command_aliases.
			free(command_aliases[index][0]);
			
			//New memory allocation for the alias using the strings length to not allocate too much.
			command_aliases[index][0] = (char *) malloc(sizeof(char) * (strlen(commandTemp)+1));

			//Copies the command to the command storage for alias.
			strcpy(command_aliases[index][0], commandTemp);

			//If the commandTemp exists, frees memory allocation for commandTemp.
			if(commandTemp)
				free(commandTemp);

			//Returns.
			return;
		}
		//If the commandTemp was null.
		else if(commandTemp == NULL)
		{
			//Prints appropriate message and returns.
			printf("No command entered to overwrite alias\n");
			return;
		}

		//Increment index.
		index++;
	}

	//If index was incremented to 10 then.
	if(index == 10)
	{
		//Print appropriate messages since no more space for a new alias and return.
		printf("Max alias limit reached\n");
		return;
	}

	//Simply loops twice, for pos 0 of index pos of matrix command_aliases, and pos 1.
	for(int i = 0; i < 2; i++)
	{
		//If i is 0, then runs as follows.
		if(i == 0)
		{
			//While commands in pos 3 is not NULL, sets temp to commandTemp.
			if(commands[3] != NULL)
				sprintf(temp, "%s", commandTemp);	
			//Otherwise, takes value in pos 2 of commands to temp.
			else
				sprintf(temp, "%s", commands[2]);
		}
		//Prints the value in pos 1 of commands to temp.
		else
			sprintf(temp, "%s", commands[1]);

		//For pos i of position index matrix of command_aliases, allocate memory appropriate as necessary.
		command_aliases[index][i] = (char *) malloc(sizeof(char) * (strlen(temp)+1));

		//copies the temp to pos i of pos index in matrix command_aliases.
		strcpy(command_aliases[index][i], temp);
	}

	//If commandTemp exists, then free memory allocation.
	if(commandTemp)
		free(commandTemp);

	//Free memory allocation of temp.
	free(temp);

	//Print that alias was successfully added.
	printf("Alias added successfully\n");
}

/*
	This method is only invoked by the check_alias method, it passes the entered commands and command for alias
	so that the command that exists can be combined with the original command plus any parameters left in commands
	before being passed to choose_process for another check. E.g. commands lsalias -l, lsalias would be replaced
	by just ls and -l would be added to the end of it, inevitably executing ls -l.
*/
void run_alias(char** commands, char* originalValue)
{
	//Initializing local variables.
	int pos = 1;
	int placedTokens = 0;

	//Creating new set of tokens.
	char* token = strtok(originalValue, Delimiter);

	//Parsing tokens to new command.
	char** nCommands = parse_input(token);

	//Finding first null position in ncommands, storing position in placedTokens.
	while(nCommands[placedTokens] != NULL)
		placedTokens++;

	//Loops while another token exists in original commands that is not null
	while(commands[pos] != NULL)
	{
		//Allocate memory to first null position in nCommands.
		nCommands[placedTokens] = (char*) malloc(sizeof(char)*strlen(commands[pos])+1);

		//Copy pos of commands to pos placedTokens of nCommands, adding token to new commands.
		strcpy(nCommands[placedTokens], commands[pos]);

		//Increment placedTokens and pos for repeating as necessary.
		placedTokens++;
		pos++;
	}

	//Attempt to choose_process again with new command.
	choose_process(nCommands);

	//Free memory allocation on nCommands.
	free(nCommands);

	//Set token to null.
	token = NULL;
}


/*
	Internal command: unalias <paramaeter>

	Removes an alias and its commands from command_aliases, making it no longer existent nor usable.
*/
void unalias(char** commands)
{
	//If there is not an alias entered to remove then
	if(commands[1] == NULL)
	{
		//Print appropriate message and return.
		printf("\nNo alias entered to remove.\n");
		return;
	}
	//While an extra token than necessary exists
	else if(commands[2] != NULL)
	{
		//Print appropriate message and return.
		printf("\nUnalias only takes one word parameter, not more.\n");
		return;
	}

	//Initializing local variable.
	int index = 0;

	//Loops while another alias exists.
	while(command_aliases[index][0] != NULL)
	{
		//If alias is found.
		if(strcmp(command_aliases[index][1], commands[1]) == 0)
		{
			//Increment index.
			index++;

			//Loop while aliases above the position of the old alias exist.
			while(command_aliases[index][0] != NULL)
			{
				//Overwrite the earlier aliases appropriately and their commands, moving them down positions.
				strcpy(command_aliases[index-1][0], command_aliases[index][0]);
				strcpy(command_aliases[index-1][1], command_aliases[index][1]);

				//Increment index.
				index++;
			}

			//Free the memory allocation of last alias position that would be where the last alias to be
			//moved up on the positions existed.
			free(command_aliases[index-1][0]);
			free(command_aliases[index-1][1]);

			//Set the values in position index-1 of command_aliases to null for both alias and command.
			command_aliases[index-1][0] = NULL;
			command_aliases[index-1][1] = NULL;
			
			//Print appropriate message.
			printf("Alias removed\n");

			//Return
			return;
		}

		//Increment index.
		index++;
	}

	//Print appropriate message.
	printf("No alias existed\n");
}

//Stops the user entering an alias that calls another alias, pointing back to initial alias.
int infinite_alias_check(char* alias, char* command)
{
	//Localizing initial variables.
	int check = 0;
	int index = 0;

	//Repeats while another alias exists.
	while(command_aliases[index][0] != NULL)
	{
		//Checks if alias passed was found to be a command elsewhere, setting check to 1 if so.
		if(strcmp(command_aliases[index][1], command) == 0 && strcmp(command_aliases[index][0], alias) == 0)
			check = 1;

		//Increment index.
		index++;
	}

	//Return check, if 1 then does not add alias, if 0 adds alias.
	return check;
}

//Loops aliases and prints them to the interface.
void print_aliases(void)
{
	//Take a new line for cleaner display.
	printf("\n");

	//Initializing local variable.
	int pos = 0;

	//Loops while another alias exists.
	while(command_aliases[pos][0] != NULL)
	{
		//Prints "<aliasname> - <command>" to display and takes new line.
		printf("%s - %s\n", command_aliases[pos][1], command_aliases[pos][0]);

		//Increment pos.
		pos++;
	}
}

//Saves aliases as the full command to create them to .alias_list file overwriting if exists, or creating if not
//they are saved in this way as they are passed to add_alias when loading them in.
int save_aliases(void)
{
	//Initializing local variables.
	int pos = 0;
  	FILE* fileP;
  	char saveCommand[Input_Max];
  	char concat[15];

  	//Set concat to new line character.
  	strcpy(concat, "\n");

  	//Set fileP to FILE pointer of .alias_list file with writable type.
  	fileP = fopen(".alias_list", "w");

  	//If opening file failed, return -1.
  	if(fileP == NULL) return -1;

  	//Repeats for all command_aliases.
  	while(command_aliases[pos][0] != NULL){

  		//Writes the full command to create the alias to the char* saveCommand.
  		sprintf(saveCommand, "alias %s %s\n", command_aliases[pos][1], command_aliases[pos][0]);

  		//Add saveCommand to the file.
    	fputs(saveCommand, fileP);

    	//Empty the value in saveCommand.
   		strcpy(saveCommand, "");

   		//Increment pos.
    	pos++;
  	}

  	//If their is no aliases, adds just a new line to file.
  	if(command_aliases[0][0] == NULL)
  		fputs("\n", fileP);

  	//Adds new line to file.
  	fputs("\n", fileP);

  	//Close file.
  	fclose(fileP);

  	//Return 0.
  	return 0;
}

//Loads aliases from the .alias_list file as their commands to create and pass them to add_alias.
int load_aliases(void)
{
	//Initializing local variables.
  	int i = 0;
  	FILE* fileP;
  	char* commands[Input_Max];
  	char* token;
  	char** tokens;
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;

  	//Sets fileP to FILE pointer of open readable .alias_list file.
  	fileP = fopen(".alias_list", "r");

  	//If fails to open file, then returns -1.
  	if (fileP == NULL) return -1;

  	//Repeats whilst another non-end of field line exists in file.
  	while ((read = getline(&line, &len, fileP)) != EOF) 
  	{
  		//Allocates memory for position i in commands.
    	commands[i] = malloc((strlen(line)+1)*sizeof(char));

    	//Sets last character of line to null character.
    	line[strlen(line)-1] = '\0';

    	//Copy the current line to commands in position i, then increment i.
    	strcpy(commands[i++], line);
  	}

  	//loops for i-1.
  	for(int x = 0; x < i-1; x++)
  	{
  		//Set token variable to strtok of command using Delimiter.
  		token = strtok(commands[x], Delimiter);

  		//if token is no null.
  		if(token)
  		{
  			//Parse token to tokens.
  			tokens = parse_input(token);

  			//Pass tokens to add_alias, to create alias.
  			add_alias(tokens);
  			
  			//Free memory allocation of position at x of commands.
  			free(commands[x]);

  			//If tokens is not null, frees memory allocation of tokens.
  			if(tokens)
  				free(tokens);
  		}

  		//Set tokens and token to null.
  		tokens = NULL;
  		token = NULL;
    }

    //Close file pointer.
    fclose(fileP);

    //If line is not null, free memory allocation of line.
  	if (line)
   		free(line);

   	//Returns 0.
  	return 0;
}
