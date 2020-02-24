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

int save_aliases(void)
{
  int pos = 0;
  FILE* fPointer;
  char holdValue[Input_Max];
  char concat[15];
  strcpy(concat, "\n");
  fPointer = fopen(".alias_list", "w");
  
  if(fPointer == NULL) return -1;

  while(command_aliases[pos][0] != NULL){
    strcpy(holdValue, strdup(command_aliases[pos][0])); 
    strcat(holdValue, concat);
    fputs(holdValue, fPointer);
    strcpy(holdValue, strdup(command_aliases[pos][1])); 
    strcat(holdValue, concat);
    fputs(holdValue, fPointer);
    pos++;
  }

  fclose(fPointer);
  return 0;
}

int load_aliases(void)
{
  	int i = 0;
  	FILE * fPointer;
  	char* strings[Input_Max];
  	char temp[100] = malloc(100*sizeof(char));
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;

  	fPointer = fopen(".alias_list", "r");
  	if (fPointer == NULL) return -1;

  	while ((read = getline(&line, &len, fPointer)) != EOF) 
  	{
    	strings[i] = malloc(100);
    	line[strlen(line)-1] = '\0';
    	strcpy(strings[i++], line);
  	}

  	int pos = 0;
  	for(int x = 0; x < i-1; x++)
  	{
  		if(x % 2 != 0)
  		{
  			strcat(temp, strcat(" ", strings[x]));
  		}

  		strcpy();
		free(strings[x]);
    }

    free(*strings);
  	fclose(fPointer);
  	if (line)
   		free(line);
  	return 0;
}
