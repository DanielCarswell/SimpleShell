#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define Input_Max 512

extern char* loopstopper;
extern char* current_history[];

void add_to_history(char temp[])
{
	if(temp == NULL || temp[0] == ' ' || temp[0] == '\r' || strcmp(temp, "\n") == 0 || temp[0] == '\e' 
		|| temp[0] == '\0' || strncmp(temp, "exit", 4) == 0)
    	return;

  	int n;
	for(int i = 0; i < 1000; i++)
  		if(current_history[i] == NULL || strcmp(current_history[i], "") == 0 || strlen(current_history[i]) == 0)
    	{
      		n = i;
      		current_history[n] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
      		break;
    	}
    	else if (i == 999)
    		n = i;

    if(n == 999)
    {
    	printf("Max history reached\n");
    	return;
    }

    if(temp[strlen(temp)-1] == '\n')
    	temp[strlen(temp)-1] = '\0';

    strcpy(current_history[n], temp);
}

int element_in_history(char** commands)
{
	int check = 0;

	if(strncmp(&commands[0][1],"\0",1) == 0)
		printf("No element of history selected\n");
	else if(strncmp(&commands[0][1],"!",1) == 0)
		check = run_history(1000);
	else if(strncmp(&commands[0][1], "-", 1) == 0)
		negative_history(commands);
	else
		positive_history(commands);

	if(check)
		return check;
	else
		return 1;
}

void positive_history(char** commands)
{
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	for(int i = 0; i < 10; i++)
		if(strncmp(&commands[0][1], chars[i], 1) == 0 && strncmp(&commands[0][2], "\0", 1) != 0)
		{
			for(int j = 0; j < 10; j++)
				if(strncmp(&commands[0][2], chars[j], 1) == 0 && strncmp(&commands[0][3], "\0", 1) != 0)
				{
					for(int x = 0; x < 10; x++)
						if(strncmp(&commands[0][4], chars[x], 1) == 0 && strncmp(&commands[0][5], "\0", 1) == 0)
						{
							printf("That element of history is out of bounds\n");
							return;
						}
						else if(strncmp(&commands[0][4], chars[x], 1) == 0 && strncmp(&commands[0][5], "\0", 1) != 0)
						{
							printf("Not a history element\n");
							return;
						}
						else if(strncmp(&commands[0][3], chars[x], 1) == 0)
							check = run_history((i*100) + (j*10) + x);
				}
				else if(strncmp(&commands[0][2], chars[j], 1) == 0)
					check = run_history((i*10) + j);
		}
		else if(strncmp(&commands[0][1], chars[i], 1) == 0)
			check = run_history(i);
		else if(i == 9 && check == 0)
			printf("Not a history element\n");
}


void negative_history(char** commands)
{
	int pos = 0;
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	while(current_history[pos] != NULL)
	{
		pos++;
	}

	for(int i = 0; i < 10; i++)
		if(strncmp(&commands[0][2], chars[i], 1) == 0 && strncmp(&commands[0][3], "\0", 1) != 0)
		{
			for(int j = 0; j < 10; j++)
				if(strncmp(&commands[0][3], chars[j], 1) == 0 && strncmp(&commands[0][4], "\0", 1) != 0)
				{
					for(int x = 0; x < 10; x++)
						if(strncmp(&commands[0][4], chars[x], 1) == 0 && strncmp(&commands[0][5], "\0", 1) == 0)
						{
							printf("That element of history is out of bounds\n");
							return;
						}
						else if(strncmp(&commands[0][4], chars[x], 1) == 0 && strncmp(&commands[0][5], "\0", 1) != 0)
						{
							printf("Not a history element\n");
							return;
						}
						else if(strncmp(&commands[0][4], chars[x], 1) == 0)
							check = run_history(((pos+1)  - ((i*100) + (j*10) + x)));
				}
				else if(strncmp(&commands[0][3], chars[j], 1) == 0)
					check = run_history(((pos+1) - ((i*10) + j)));
		}
		else if(strncmp(&commands[0][2], chars[i], 1) == 0)
			check = run_history(((pos+1) - i));
		else if(i == 9 && check == 0)
			printf("Not a history element\n");
}

int run_history(int i)
{
	if(i < 0)
	{
		printf("Negative history elements do not exist\n");
		return 1;
	}
	
	int pos = 0;
	
	if(i == 1000)
	{
		while(current_history[pos] != NULL)
			pos++;
	
		i = pos;
	}

	i--;

	if(current_history[i] != NULL)
	{
		if(strcmp(current_history[i], loopstopper) == 0)
		{
			printf("Loop detected, cancelling command...\n");
			sprintf(loopstopper, "%s", "/0");
			return 1;
		}
		printf("Running command: %s\n", current_history[i]);
		sprintf(loopstopper, "%s", current_history[i]);
		char* token = strtok(strdup(current_history[i]), " \n");
		char** ncommands = parse_input(token);

		while(token)
		{
			choose_process(ncommands);
			free(ncommands);
			token = NULL;
		}
	}
	else
		printf("No element in history\n");

	sprintf(loopstopper, "%s", "/0");
	return 1;
}

void print_history(char ** commands)
{
	int i;
	for(i = 0; i < 1000; i++)
	{
		if(current_history[i] == NULL)
			break;

		printf("%d. %s\n", (i+1), current_history[i]);
	}

	printf("%d. history\n", (i+1));
}

int save_history(void)
{
	int pos = 0;
    FILE* fileP;
    char saveString[Input_Max];
    char concat[15];
    strcpy(concat, "\n");
    fileP = fopen(".hist_list", "w");
  
    if(fileP == NULL) return -1;

    while(current_history[pos] != NULL) {
    	sprintf(saveString, "%s\n", current_history[pos]);  
    	fputs(saveString, fileP);
    	pos++;
  	}

  	fputs(concat, fileP);
  	fclose(fileP);
  	return 0;
}

int load_history(void)
{
	FILE* fileP;
  	char* commands[Input_Max];
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;
  	int i = 0;

  	fileP = fopen(".hist_list", "r");
  	if (fileP == NULL) return -1;

  	while ((read = getline(&line, &len, fileP)) != EOF) 
  	{
    	commands[i] = malloc((strlen(line)+1)*sizeof(char));
    	line[strlen(line)-1] = '\0';
    	strcpy(commands[i++], line);	
  	}

  	for(int x = 0; x < i-1; x++)
  	{
    	add_to_history(commands[x]);
    	free(commands[x]);
	}

  	fclose(fileP);
 	if (line)
    	free(line);

  	return 0;
}