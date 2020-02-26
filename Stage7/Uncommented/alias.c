#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define Input_Max 512

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
	
	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][0], commands[2]) == 0)
		{
			printf("Old alias overwritten with: %s\n", commands[1]);
			sprintf(temp, "%s", commands[1]);
			free(command_aliases[index][1]);
			command_aliases[index][1] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
			strcpy(command_aliases[index][1], temp);
			return 0;
		}
		index++;
	}

	if(index == 10)
	{
		printf("Max alias limit reached\n\n");
		return 0;
	}

	for(int i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			if(commands[3] != NULL)
			{
				char* commandTemp;
				commandTemp = tokens_to_line(commands);
				sprintf(temp, "%s", commandTemp);	
			}
			else
			sprintf(temp, "%s", commands[2]);
		}
		else
			sprintf(temp, "%s", commands[1]);

		command_aliases[index][i] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
		strcpy(command_aliases[index][i], temp);
	}

	free(temp);
	return 0;
}


void run_alias(char** commands, char* originalValue)
{
	int pos = 1;
	int placedTokens = 0;
	char* token = strtok(originalValue, " ");
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

char* tokens_to_line(char** commands)
{
	int pos = 2;
	char* line = malloc(1);
	line = NULL;

	while(commands[pos] != NULL)
	{
		int buffer = 512;
		line = realloc(line, buffer);
		if(pos == 2)
			sprintf(line, "%s", commands[pos]);
		else
			sprintf(line, "%s %s", line, commands[pos]);
		pos++;
	}

	return line;
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
	printf("\n");
}