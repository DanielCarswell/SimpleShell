#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define Input_Max 512
#define Delimiter " \n\t;&><|"

extern char* internal_commands[];
extern char* command_aliases[10][2];


int check_alias(char** commands)
{
	int pos = 0;
	while(command_aliases[pos][0] != NULL)
	{
		if(strcmp(command_aliases[pos][1], commands[0]) == 0)
		{
			run_alias(commands, strdup(command_aliases[pos][0]));
			return 1;
		}
		pos++;
	}

	return 0;
}

int add_alias(char** commands)
{
	char* commandTemp;
	char* temp = (char*) malloc(sizeof(char)*Input_Max);
	int index = 0;

	for(int i = 0; i < 3; i ++)
	{
		if(commands[i] == NULL)
		{
			if(i == 1)
				print_aliases();
			else 
				printf("Not enough arguments\n");
			return 0;
		}
	}

	commandTemp = command_line(commands, 2);

	if(infinite_alias_check(commands[1], commandTemp))
	{
		printf("This alias would cause an infinite loop and was not added.\n");
		return 0;
	}
	
	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][0], commandTemp) == 0 && strcmp(command_aliases[index][1], commands[1]) != 0)
		{
			printf("Old alias overwritten with: %s\n", commands[1]);
			sprintf(temp, "%s", commands[1]);
			free(command_aliases[index][1]);
			command_aliases[index][1] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
			strcpy(command_aliases[index][1], temp);
			if(commandTemp)
				free(commandTemp);
			return 0;
		}
		else if(strcmp(command_aliases[index][1], commands[1]) == 0)
		{
			printf("This alias already exists for that command\n");
			return 0;
		}
		index++;
	}

	if(index == 10)
	{
		printf("Max alias limit reached\n");
		return 0;
	}

	for(int i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			if(commands[3] != NULL)
				sprintf(temp, "%s", commandTemp);	
			else
				sprintf(temp, "%s", commands[2]);
		}
		else
			sprintf(temp, "%s", commands[1]);

		command_aliases[index][i] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
		strcpy(command_aliases[index][i], temp);
	}

	if(commandTemp)
		free(commandTemp);
	free(temp);
	return 0;
}


void run_alias(char** commands, char* originalValue)
{
	int pos = 1;
	int placedTokens = 0;
	char* token = strtok(originalValue, Delimiter);
	char** nCommands = parse_input(token);

	while(nCommands[placedTokens] != NULL)
		placedTokens++;

	while(commands[pos] != NULL)
	{
		nCommands[placedTokens] = (char*) malloc(sizeof(char)*strlen(commands[pos])+1);
		strcpy(nCommands[placedTokens], commands[pos]);
		placedTokens++;
		pos++;
	}

	choose_process(nCommands);
	free(nCommands);
	token = NULL;
}

int unalias(char** commands)
{
	if(commands[1] == NULL)
	{
		printf("\nNo alias entered to remove.\n");
		return 0;
	}
	else if(commands[2] != NULL)
	{
		printf("\nUnalias only takes one word parameter, not more.\n");
		return 0;
	}
	int index = 0;
	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][1], commands[1]) == 0)
		{
			index++;
			while(command_aliases[index][0] != NULL)
			{
				strcpy(command_aliases[index-1][0], command_aliases[index][0]);
				strcpy(command_aliases[index-1][1], command_aliases[index][1]);
				index++;
			}
			free(command_aliases[index-1][0]);
			free(command_aliases[index-1][1]);

			command_aliases[index-1][0] = NULL;
			command_aliases[index-1][1] = NULL;
			
			printf("Alias removed\n");
			return 0;
		}
		index++;
	}

	printf("No alias existed\n");

	return 0;
}

int infinite_alias_check(char* alias, char* command)
{
	int check = 0;
	int index = 0;

	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][1], command) == 0 && strcmp(command_aliases[index][0], alias) == 0)
			check = 1;
		index++;
	}

	return check;
}

void print_aliases(void)
{
	printf("\n");
	int pos = 0;
	while(command_aliases[pos][0] != NULL)
	{
		printf("%s - %s\n", command_aliases[pos][1], command_aliases[pos][0]);
		pos++;
	}
}