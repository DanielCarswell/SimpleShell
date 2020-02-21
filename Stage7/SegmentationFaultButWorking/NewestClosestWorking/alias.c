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
			char* token;
			replace_alias(commands, strdup(command_aliases[pos][0]));
			char* fixcommand = charpointertoarray(commands, 1);
			token = strtok(strdup(fixcommand), " \n\t;&><|");
			char** ncommands = parseInput(token);
			int status = runProcess(ncommands);
			int pos = 0;
			while(ncommands[pos] != NULL)
			{
				free(ncommands[pos]);
				pos++;	
			}

			free(ncommands);
			free(fixcommand);
			fixcommand = NULL;
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


void replace_alias(char** commands, char* originalValue)
{
	int placedTokens = 0;
	int pos = 0;
	int buffer = 100;
	char** newCommand = parseInput(originalValue);


	while(newCommand[pos] != NULL)
		pos++;

	placedTokens = pos;
	pos = 1;
	printf("%d", placedTokens);

	while(commands[pos] != NULL)
	{
		if(pos>=buffer) {
			buffer += 100;
			newCommand = realloc(newCommand, buffer);
		}
		strcpy(newCommand[placedTokens], commands[pos]);
		placedTokens++;
		pos++;
	}

	int memPos = 0;
	while(commands[memPos] != NULL)
	{
		free(commands[memPos]);
		memPos++;	
	}
	free(commands);
	commands = NULL;

	commands = (char**) malloc(sizeof(char*) * buffer);

	int copyPos = 0;
	while(newCommand[copyPos] != NULL)
	{
		commands[copyPos] = (char*) malloc(sizeof(char) * strlen(newCommand[copyPos]));
		strcpy(commands[copyPos], newCommand[copyPos]);
		copyPos++;
	}

	add_to_history(commands[0]);

	memPos = 0;
	while(newCommand[memPos] == NULL)
	{
		free(newCommand[memPos]);
		memPos++;	
	}
	free(newCommand);
	newCommand = NULL;
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
			strcpy(commandToAlias, "");
			printf("%s\n", commands[pos]);
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