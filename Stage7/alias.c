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
			char* fixcommand = (char *) malloc(sizeof(char) * 512);
			char* token = (char *) malloc(sizeof(char) * 512);
			//printf("hit: %s", command_aliases[pos][0]);
			int status = replace_alias(commands, command_aliases[pos][0]);
			fixcommand = charpointertoarray2(commands);
			token = strtok(fixcommand, " \n\t;&><|");
			commands = parseInput(token);
			//printTokens(commands);
			//free(token);
			free(fixcommand);
			//token = NULL;
			strcpy(fixcommand, "");
			printf("%s\n", fixcommand);
			return status;
		}
		pos++;
	}

	return 0;
}

int add_alias(char** commands)
{
	char temp[Input_Max];
	int index = -1;

	//Should be atleast 3 tokens for this command, can't run otherwise.
	for(int i = 0; i < 3; i ++)
		if(commands[i] == NULL)
		{
			printf("Not enough arguments\n");
			return 0;
		}

	/*for(int i = 0; i < internalCommandsCount(); i++)
	{
		if(strcmp(internal_commands[i], commands[2]) == 0)
		{
			printf("ho ho ho: %d", i);
			index = i;
		}
	}*/
	//if(index == 0)
		//index = check_external_commands(*commands);

	/*if(index == -1)
	{
		/	/printf("oh no");
		return 0;
	}*/

	index = 0;
	while(command_aliases[index][0] != NULL)
	{
		//printf("thanks god");
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

int replace_alias(char** commands, char* originalValue)
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
	printf("%d", placedTokens);

	//While loop appears to break currently. abcdef
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

	commands = (char**) malloc(sizeof(char*) * buffer);

	int copyPos = 0;
	while(newCommand[copyPos] != NULL)
	{
		//printf("This: %s", newCommand[copyPos]);
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
	newCommand = NULL;

	return 0;
}

char* charpointertoarray(char** commands)
{
	int pos = 2;
	char* commandToAlias = (char*) malloc(sizeof(char)*512);
	while(commands[pos] != NULL)
	{
		printf("%s\n", commands[pos]);
		int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos == 2)
			strcat(commandToAlias, commands[pos]);
		else
			strcat(strcat(commandToAlias, " "), commands[pos]);
		pos++;
	}

	printf("%d%s\n", 5, commandToAlias);

	return commandToAlias;
}

char* charpointertoarray2(char** commands)
{
	int pos = 0;
	char* commandToAlias = malloc(1);
	while(commands[pos] != NULL && commands[pos+1] != NULL )
	{
		printf("%s\n", commands[pos]);
		int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos!= 0)
			strcat(strcat(commandToAlias, " "), commands[pos]);
		else
			strcat(commandToAlias, commands[pos]);
		pos++;
	}

	printf("%d%s\n", 5, commandToAlias);

	return commandToAlias;
}

/*int check_external_commands(char commands[])
{
	char testCmd[100];
	char usrCmd[len(commands)-1];

	strcpy(testCmd, "which ");
	strcpy(usrCmd, commands);
	strncat(testCmd, usrCmd, 93);

	char path[100];
	FILE *p = popen( testCmd , "r");
	if(p) {
    	while(fgets(path, sizeof(path), p) != NULL) {
        	printf("%s", path);
    	}
    	for(int i = 0; i < 3; i++)
    		if(strcmp(path, "which: ") != 0 || (strcmp(path, "which: ") && strcmp(path, "invalid") && strcmp(path, "option")))
	}
}*/
