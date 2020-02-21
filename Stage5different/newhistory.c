#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define Input_Max 512

extern char* current_history[];

int add_to_history(char temp[])
{
  if(temp == NULL || temp[0] == ' ' || temp[0] == '\r' || strcmp(temp, "\n") == 0 || temp[0] == '\e' 
  	|| (temp[0] == 'e' && temp[1] == 'x' && temp[2] == 'i' && temp[3] == 't' ))
    return 0;

  int n;

  for(int i = 0; i < 500; i++)
    if(current_history[i] == NULL || strcmp(current_history[i], "") == 0 || strlen(current_history[i]) == 0 || i == 99)
    {
      n = i;
      current_history[n] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
      break;
    }

    if(n == 500)
    	printf("Max history reached\n");
    else
    	strcpy(current_history[n], temp);
    return 1;
}

int element_in_history(char** commands)
{
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	//Uses strncmp for comparing an nth element, instead of strcmp
	//which compares a full string.
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
								if(strncmp(&commands[0][3], "\0", 1) == 0)
									check = run_history((i*10) + j);
						
						if(strncmp(&commands[0][3], "\0", 1) == 0)
							check = run_history((i*10) + j);
						else
							for(int x = 0; x < 10; x++)
							{
								if(strncmp(&commands[0][3], chars[x], 1) == 0)
									if(strncmp(&commands[0][4], "\0", 1) == 0)
									{
										check = run_history((x*100) + (i*10) + j);
										return 1;
									}
									else
									{
										printf("No element in history\n");
										return 1;
									}
							}
						}
				else if(i == 9 && check == 0 && strncmp(&commands[0][3], "\0", 1) == 0)
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
		char** ncommands = parseInput(ab);
		int i = runProcess(ncommands);
		free(ab);

		if(i == 0)
			return 1;
		else 
			return i;
	}
	else
		printf("No element in history\n");
	return 1;
}



int print_history(char ** commands)
{
	for(int i = 0; i < 100; i++)
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
  FILE* fPointer;
  char holdValue[Input_Max];
  char concat[15];
  strcpy(concat, "\n");
  fPointer = fopen(".hist_list", "w");
  
  if(fPointer == NULL) return -1;

  while(current_history[pos] != NULL){
    strcpy(holdValue, current_history[pos]);  
    strcat(holdValue, concat);
    fputs(holdValue, fPointer);
    pos++;
  }

  fclose(fPointer);
  return 0;
}

int load_history(void)
{
  int i = 0;
  FILE * fPointer;
  char* strings[Input_Max];
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  fPointer = fopen(".hist_list", "r");
  if (fPointer == NULL) return -1;

  while ((read = getline(&line, &len, fPointer)) != EOF) 
  {
    strings[i] = malloc(100);
    line[strlen(line)-1] = '\0';
    strcpy(strings[i++], line);
  }

  for(int x = 0; x < i-1; x++)
    add_to_history(strings[x]);

  fclose(fPointer);
  if (line)
    free(line);
  return 0;
}