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
			recursive_check_alias(commands, command_aliases[pos][1]);
			run_alias(commands, strdup(command_aliases[pos][0]));
			return 1;
		}
		pos++;
	}

	return 0;
}

int recursive_check_alias(char** commands, char* alias)
{
	int position = 0;
	while(command_aliases[position][1] != NULL)
	{
		if(strcmp(alias, command_aliases[position][0]) == 0)
		{
			strcpy(commands[0], command_aliases[position][1]);
			recursive_check_alias(commands, command_aliases[position][1]);
		}
		position++;
	}

	return 0;
}

void add_alias(char** commands)
{
	if(strncmp(commands[0], "alias", 5) != 0)
	{
		printf("\n");
		return;
	}

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
			return;
		}
	}

	commandTemp = command_line(commands, 2);

	if(infinite_alias_check(commands[1], commandTemp))
	{
		printf("This alias would cause an infinite loop and was not added.\n");
		return;
	}
	
	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][1], commands[1]) == 0 && commandTemp != NULL)
		{
			printf("Old alias overwritten, new command: %s\n", commandTemp);
			free(command_aliases[index][0]);
			
			command_aliases[index][0] = (char *) malloc(sizeof(char) * (strlen(commandTemp)+1));
			strcpy(command_aliases[index][0], commandTemp);

			if(commandTemp)
				free(commandTemp);

			return;
		}
		else if(commandTemp == NULL)
		{
			printf("No command entered to overwrite alias\n");
			return;
		}
		else if(strcmp(command_aliases[index][1], commands[1]) == 0)
		{
			printf("This alias already exists for that command\n");
			return;
		}
		index++;
	}

	if(index == 10)
	{
		printf("Max alias limit reached\n");
		return;
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
	printf("Alias added successfully\n");
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

void unalias(char** commands)
{
	if(commands[1] == NULL)
	{
		printf("\nNo alias entered to remove.\n");
		return;
	}
	else if(commands[2] != NULL)
	{
		printf("\nUnalias only takes one word parameter, not more.\n");
		return;
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
			return;
		}
		index++;
	}

	printf("No alias existed\n");
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

int save_aliases(void)
{
  int pos = 0;
  FILE* fileP;
  char saveCommand[Input_Max];
  char concat[15];
  strcpy(concat, "\n");
  fileP = fopen(".alias_list", "w");
  
  if(fileP == NULL) return -1;

  while(command_aliases[pos][0] != NULL){
  	sprintf(saveCommand, "alias %s %s\n", command_aliases[pos][1], command_aliases[pos][0]);
    fputs(saveCommand, fileP);
    strcpy(saveCommand, "");
    pos++;
  }

  if(command_aliases[0][0] == NULL)
  	fputs("\n", fileP);

  fputs("\n", fileP);
  fclose(fileP);

  return 0;
}

int load_aliases(void)
{
  	int i = 0;
  	FILE* fileP;
  	char* commands[Input_Max];
  	char* token;
  	char** tokens;
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;

  	fileP = fopen(".alias_list", "r");
  	if (fileP == NULL) return -1;

  	while ((read = getline(&line, &len, fileP)) != EOF) 
  	{
    	commands[i] = malloc(100);
    	line[strlen(line)-1] = '\0';
    	strcpy(commands[i++], line);
  	}

  	for(int x = 0; x < i-1; x++)
  	{
  		token = strtok(commands[x], Delimiter);

  		if(token)
  		{
  			tokens = parse_input(token);
  			add_alias(tokens);
  			
  			free(commands[x]);

  			if(tokens)
  				free(tokens);
  		}
  			
  		tokens = NULL;
  		token = NULL;
    }

    fclose(fileP);

  	if (line)
   		free(line);

  	return 0;
}
