#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define Input_Max 512

extern char* internal_commands[];
extern char* command_aliases[99][2];


int check_alias(char** commands)
{
	int pos = 0;
	while(command_aliases[pos][0] != NULL)
	{
		if(strcmp(command_aliases[pos][1], commands[0]) == 0)
		{
			char* fixcommand;
			char* token = (char*) malloc(sizeof(char)*Input_Max);
			replace_alias(commands, strdup(command_aliases[pos][0]));
			fixcommand = charpointertoarray2(commands);
			token = strtok(strdup(fixcommand), " \n\t;&><|");
			char** ncommands = parseInput(token);
			runProcess(ncommands);
			int memPos = 0;
			while(ncommands[memPos] != NULL)
			{
				free(ncommands[memPos]);
				memPos++;	
			}

			free(ncommands);
			strcpy(fixcommand, "");
			token = NULL;
			return 1;
		}
		pos++;
	}

	return 0;
}

int add_alias(char** commands)
{
	char temp[Input_Max];
	int index = -1;

	for(int i = 0; i < 3; i ++)
		if(commands[i] == NULL)
		{
			printf("Not enough arguments\n");
			return 0;
		}

	index = 0;
	while(command_aliases[index][0] != NULL)
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

	for(int i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			if(commands[3] != NULL)
			{
				char* commandTemp;
				commandTemp = charpointertoarray(commands);
				sprintf(temp, commandTemp);
				free(commandTemp);
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

void replace_alias(char** commands, char* originalValue)
{
	int placedTokens = 0;
	int pos = 0;
	int buffer = 100;
	char** newCommand = parseInput(originalValue);

	while(newCommand[pos] != NULL)
	{
		pos++;
	}

	placedTokens = pos;
	pos = 1;

	while(commands[pos] != NULL)
	{
		newCommand[placedTokens] = (char*) malloc(sizeof(check_external_commands)*20);
		strcpy(newCommand[placedTokens], commands[pos]);
		placedTokens++;
		pos++;
	}

	pos = 0;
	while(commands[pos] != NULL)
	{
		free(commands[pos]);
		pos++;
	}

	int memPos = 0;
	while(commands[memPos] == NULL)
	{
		free(commands[memPos]);
		memPos++;	
	}
	free(commands);
	commands = NULL;

	commands = newCommand;

	/*int copyPos = 0;
	while(newCommand[copyPos] != NULL)
	{
		commands[copyPos] = (char*) malloc(sizeof(char) * 50);
		strcpy(commands[copyPos], newCommand[copyPos]);
		copyPos++;
	}

	memPos = 0;
	while(newCommand[memPos] == NULL)
	{
		free(newCommand[memPos]);
		memPos++;	
	}
	free(newCommand);
	newCommand = NULL;*/
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

char* charpointertoarray(char** commands)
{
	int pos = 2;
	char* commandToAlias = malloc(1);
	while(commands[pos] != NULL)
	{
		strcpy(commandToAlias, "");
		int buffer = strlen(commands[pos]) + strlen(commandToAlias) + 2;
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos == 2)
			strcat(commandToAlias, commands[pos]);
		else
			strcat(strcat(commandToAlias, " "), commands[pos]);
		pos++;
	}

	return commandToAlias;
}

char* charpointertoarray2(char** commands)
{
	int pos = 0;
	char* commandToAlias = malloc(1);
	while(commands[pos] != NULL && commands[pos+1] != NULL )
	{
		strcpy(commandToAlias, "");
		printf("%s\n", commands[pos]);
		int buffer = strlen(commands[pos]) + strlen(commandToAlias) + 2;
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos!= 0)
			strcat(strcat(commandToAlias, " "), commands[pos]);
		else
			strcat(commandToAlias, commands[pos]);
		pos++;
	}
	return commandToAlias;
}