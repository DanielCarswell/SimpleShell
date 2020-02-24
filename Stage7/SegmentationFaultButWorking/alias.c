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
	char temp[Input_Max];
	int index = 0;

	for(int i = 0; i < 3; i ++)
		if(commands[i] == NULL)
		{
			printf("Not enough arguments\n");
			return 0;
		}

	while(index != 10 || command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][0], commands[2]) == 0)
		{
			printf("Old alias overwritten with: %s\n", commands[1]);
			sprintf(temp, commands[1]);
			free(command_aliases[index][1]);
			command_aliases[index][1] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
			strcpy(command_aliases[index][1], temp);
			return 0;
		}
		index++;
	}

	if(index == 10)
		index = 9;

	for(int i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			if(commands[3] != NULL)
			{
				char* commandTemp;
				commandTemp = charpointertoarray(commands, 0);
				sprintf(temp, commandTemp);	
			}
			else
			sprintf(temp, commands[2]);
		}
		else
			sprintf(temp, commands[1]);

		command_aliases[index][i] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
		strcpy(command_aliases[index][i], temp);
	}
	return 0;
}


void run_alias(char** commands, char* originalValue)
{
	int placedTokens = 0;
	int pos = 1;
	char* token = strtok(originalValue, " ");
	char** newCommand = parseInput(token);


	while(newCommand[placedTokens] != NULL)
		placedTokens++;

	while(commands[pos] != NULL)
	{
		newCommand[placedTokens] = (char*) malloc(sizeof(char)*strlen(commands[pos])+1);
		strcpy(newCommand[placedTokens], commands[pos]);
		placedTokens++;
		pos++;
	}

	runProcess(newCommand);
	token = NULL;
}

int unalias(char** commands)
{
	int index = 0;
	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][1], commands[1]) == 0)
		{
			command_aliases[index][0] = NULL;
			command_aliases[index][1] = NULL;
			printf("Alias removed\n");
			return 0;
		}
		index++;
	}

	printf("No alias existed\n");
	return 0;
}

char* charpointertoarray(char** commands, int choice)
{
	int pos = 2;
	char* commandToAlias = malloc(1);
	strcpy(commandToAlias, "");
	if(choice == 0)
	while(commands[pos] != NULL)
	{

		int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos == 2)
			strcat(commandToAlias, commands[pos]);
		else
			strcat(strcat(commandToAlias, " "), commands[pos]);
		pos++;
	}
	else
	{
		pos = 0;
		while(commands[pos] != NULL && commands[pos+1] != NULL )
		{
			int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
			commandToAlias = realloc(commandToAlias, buffer);
			if(pos!= 0)
				strcat(strcat(commandToAlias, " "), commands[pos]);
			else
				strcat(commandToAlias, commands[pos]);
			pos++;
		}
	}

	return commandToAlias;
}