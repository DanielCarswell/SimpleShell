#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define Input_Max 512

extern char* current_history[];

void add_to_history(char temp[])
{
	if(temp == NULL || temp[0] == ' ' || temp[0] == '\r' || strcmp(temp, "\n") == 0 || temp[0] == '\e' 
		|| (temp[0] == 'e' && temp[1] == 'x' && temp[2] == 'i' && temp[3] == 't' ))
    	return;

  	int n;
	for(int i = 0; i < 1000; i++)
  		if(current_history[i] == NULL || strcmp(current_history[i], "") == 0 || strlen(current_history[i]) == 0 || i == 999)
    	{
      		n = i;
      		current_history[n] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
      		break;
    	}

    if(n == 999)
    	printf("Max history reached");

    strcpy(current_history[n], temp);
}

int element_in_history(char** commands)
{
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	if(strncmp(commands[0], "!", 1) == 0)
	{
		if(strncmp(&commands[0][1],"\0",1) == 0)
			printf("No element of history selected\n");
		else if(strncmp(&commands[0][1],"!",1) == 0)
			check = run_history(0);
		else
			for(int i = 0; i < 10; i++)
			{
				if(strncmp(&commands[0][1], chars[i], 1) == 0)
					if(strncmp(&commands[0][2], "\0", 1) == 0)
						check = run_history(i);
					else
						for(int j = 0; j < 10; j++)
						{
							if(strncmp(&commands[0][2], chars[j], 1) == 0)
							{
								if(strncmp(&commands[0][3], "\0", 1) == 0)
									check = run_history((i*10) + j);
								else
									for(int x = 0; x < 6; x++)
									{
										if(strncmp(&commands[0][3], chars[x], 1) == 0)
										{
											if(strncmp(&commands[0][4], "\0", 1) == 0)
												check = run_history((i*100) + (j*10) + x);
											else
											{
												printf("That element of history is out of bounds\n");
												return 1;
											}
										}
									}
								}
						}
				else if(i == 9 && check == 0 && strncmp(&commands[0][4], "\0", 1) == 0)
				{
					printf("Invalid character after !\n");
					break;
				}
			}
	}

	return check;
}

int run_history(int i)
{
	int pos = 0;
	
	if(i == 0)
		while(current_history[pos] != NULL)
		{
			pos++;
			i = pos;
		}

	if(current_history[i-1] != NULL)
	{
		printf("Running command: %s\n", current_history[i-1]);

		char* ab = strtok(strdup(current_history[i-1]), " \n");
		char** ncommands = parse_input(ab);
		choose_process(ncommands);
		free(ncommands);
		ab = NULL;
	}
	else
		printf("No element in history\n");

	return 1;
}

int print_history(char ** commands)
{
	for(int i = 0; i < 1000; i++)
	{
		if(current_history[i] == NULL)
			break;

		printf("%d. %s\n", (i+1), current_history[i]);
	}

	return 0;
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
    	strcpy(saveString, current_history[pos]);  
    	strcat(saveString, concat);
    	fputs(saveString, fileP);
    	pos++;
  	}

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
    	commands[i] = malloc(100);
    	line[strlen(line)-1] = '\0';
    	strcpy(commands[i++], line);
  	}

  	for(int x = 0; x < i-1; x++)
  	{
    	add_to_history(commands[x]);
    	free(commands[x]);
	}

	free(*commands);
  	fclose(fileP);
 	if (line)
    	free(line);

  	return 0;
}