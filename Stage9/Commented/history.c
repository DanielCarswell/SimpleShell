#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Definition for reused value, 512 is input limit.
#define Input_Max 512

//Getting necessary external global variables for this module.
extern char* loopstopper;
extern char* current_history[];

/*
	Attempts to add last command entered by user to history, will check for any weird commands entered 
	and will return if they are spotted to stop funny inputs that will damage the system being added to history.
*/
void add_to_history(char temp[])
{
	//Checks for different instances where we do not want to add the value to history and returns if so.
	if(temp == NULL || temp[0] == ' ' || temp[0] == '\r' || strcmp(temp, "\n") == 0 || temp[0] == '\e' 
		|| temp[0] == '\0' || strncmp(temp, "exit", 4) == 0)
    	return;

    //Initialize local variable.
  	int n;

  	//Loops up to 1000 times, until an empty place in history is found, or i is 999.
	for(int i = 0; i < 1000; i++)
  		if(current_history[i] == NULL || strcmp(current_history[i], "") == 0 || strlen(current_history[i]) == 0)
    	{
    		//Sets n = i and allocates memory to 
      		n = i;
      		current_history[n] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
      		break;
    	}
    	//If it runs and does not find an empty element by 999th iteration, simply sets n = i.
    	else if (i == 999)
    		n = i;

    //runs if n is 999.
    if(n == 999)
    {
    	//Prints appropriate message and returns, no history element added.
    	printf("Max history reached\n");
    	return;
    }

    //If the temp closes with a new line, set that character to null,
    //otherwise it will create spaces in display for history.
    if(temp[strlen(temp)-1] == '\n')
    	temp[strlen(temp)-1] = '\0';

    //Adds temp to current_history in position n.
    strcpy(current_history[n], temp);
}

/*
	This function does a lot of checks and depending on what conditions it meets,
	it will run the appropriate function or method.
*/
int element_in_history(char** commands)
{
	//Initializing local variable.
	int check = 0;

	//If 2nd character of token is null and just '!' was entered.
	if(strncmp(&commands[0][1],"\0",1) == 0)
		//Prints appropriate message.
		printf("No element of history selected\n");

	/*If token "!!" was entered, passes a value that would otherwise not be passed to run history
	and this will trigger it to find and run last element of history.*/
	else if(strncmp(&commands[0][1],"!",1) == 0)
		check = run_history(1000);

	//If 2nd element of token was a '-' then we attempt to find the position in relation to the end of history.
	else if(strncmp(&commands[0][1], "-", 1) == 0)
		negative_history(commands);

	//Otherwise if no other statement was hit, then attempt to find the position in history from start.
	else
		positive_history(commands);

	//If check then returns check otherwise return 1.
	if(check)
		return check;
	else
		return 1;
}

/*
	If appropriate input, then will find the number associated to history element and then try run it,
	otherwise it will display an appropriate message about why it failed to user.
*/
void positive_history(char** commands)
{
	//Initializing local variables.
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	/*
	This is quite chunky and would look a mess to comment lines individually so I will try explain it all together,
	it will loop necessary depending on the amount of numbers tagged onto the '!' command and call print an appropriate
	message when fails or call the run_history using the value it was found to be by passing appropriate number.
	*/
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

/*
	Just like the positive history, it will display an appropriate message if fails, but it will attempt
	to find the number at the end of the input using positions and comparing to char[], if it finds an appropriate
	number then it will pass the position of last element in history minus the number to run_history.
*/
void negative_history(char** commands)
{
	//Initializing local variables.
	int pos = 0;
	int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	//While another value of current_history exists, increments pos.
	while(current_history[pos] != NULL)
		pos++;

	//Works very similar to the positive_history check, read that definition for a better understanding,
	//this is different because it passes the size of history minus the number found to run_history.
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

/*
	the run_history function takes in a position of history and then does various checks depending on what
	number was passed in to determine if it is running last element of history, if the element does not exist
	in history or the history element at position passed.
*/
int run_history(int i)
{
	// if the passed position is less than 0, then not a history element.
	if(i < 0)
	{
		//Prints appropriate message and returns 1.
		printf("Negative history elements do not exist\n");
		return 1;
	}
	
	//Initializing local variable.
	int pos = 0;
	
	//If i is 1000, which will only be passed if '!!' which is for last history element.
	if(i == 1000)
	{
		//loops til final position of history is found.
		while(current_history[pos] != NULL)
			pos++;
	
		//Sets i to final position of history.
		i = pos;
	}

	//Decrements i, this is because element in history is 1 greater than actual stored value,
	//since displayed is 1 as start, and in code is 0 as start.
	i--;

	//Whilst position at i of current_history is not null.
	if(current_history[i] != NULL)
	{

		//If the current_history element matches value of loopstopper.
		if(strcmp(current_history[i], loopstopper) == 0)
		{
			//Prints that a loop was detected, rests loopstopper and returns 1.
			printf("Loop detected, cancelling command...\n");
			sprintf(loopstopper, "%s", "/0");
			return 1;
		}

		//Prints what command of history is being run, and sets loopstopper to that value.
		printf("Running command: %s\n", current_history[i]);
		sprintf(loopstopper, "%s", current_history[i]);

		//Create token for history element.
		char* token = strtok(strdup(current_history[i]), " \n");

		//Parse token for a new input.
		char** ncommands = parse_input(token);

		//While token exists.
		while(token)
		{
			//Pass new commands to choose_process to do a new check on that history element
			//so that it can run the command appropriately.
			choose_process(ncommands);

			//Free the memory allocation of ncommands.
			free(ncommands);

			//Set token to null, this will also exit loop.
			token = NULL;
		}
	}
	//otherwise, print appropriate message.
	else
		printf("No element in history\n");

	//Set the value of loopstopper to null and return 1.
	sprintf(loopstopper, "%s", "/0");
	return 1;
}

//Prints all of the elements in current_history.
void print_history(char ** commands)
{
	//Initialising local variable.
	int i;

	//Repeating for 0-999.
	for(i = 0; i < 1000; i++)
	{
		//If the element at position i of current_history is null, then breaks loop.
		if(current_history[i] == NULL)
			break;

		//Prints history element with related number to position.
		printf("%d. %s\n", (i+1), current_history[i]);
	}

	//When running history command, shows history was called as it will not be added til after execution.
	printf("%d. history\n", (i+1));
}

//Saves history elements to a .hist_list file, overwriting if exists, or creating if not.
int save_history(void)
{
	//Initializing local variables.
	int pos = 0;
    FILE* fileP;
    char saveString[Input_Max];
    char concat[15];

    //Setting concat value to just a new line character.
    strcpy(concat, "\n");

    //Sets FILE pointer to write mode .hist_list file.
    fileP = fopen(".hist_list", "w");
  
  	//If it fails to open file, then returns -1.
    if(fileP == NULL) return -1;

    //Repeats for all elements in current_history.
    while(current_history[pos] != NULL) {

    	//Copies value in position at pos of current_history to saveString variable.
    	sprintf(saveString, "%s\n", current_history[pos]);  

    	//Add the saveString value to the file.
    	fputs(saveString, fileP);

    	//Increment pos.
    	pos++;
  	}

  	//Add the concat value to file, this will just add an extra line as without would read 1 less elements than necessary.
  	fputs(concat, fileP);

  	//Closes file.
  	fclose(fileP);

  	//Returns 0;
  	return 0;
}

//Loads history elements from the .hist_list file.
int load_history(void)
{
	//Initializing local variables.
	FILE* fileP;
  	char* commands[Input_Max];
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;
  	int i = 0;

  	//Sets FILE pointer to instance of opened readable .hist_list file.
  	fileP = fopen(".hist_list", "r");

  	//Returns -1 if fails to open file.
  	if (fileP == NULL) return -1;

  	//Repeats whilst another non-end of field line exists.
  	while ((read = getline(&line, &len, fileP)) != EOF) 
  	{
  		//Allocates memory for position i in commands.
    	commands[i] = malloc((strlen(line)+1)*sizeof(char));

    	//Sets last character of line to null character.
    	line[strlen(line)-1] = '\0';

    	//Copy the current line to commands in position i, then increment i.
    	strcpy(commands[i++], line);	
  	}

  	//Loops for i - 1.
  	for(int x = 0; x < i-1; x++)
  	{
  		//Attempts to add value at position x of commands to history.
    	add_to_history(commands[x]);

    	//Frees memory allocation at position x of commands.
    	free(commands[x]);
	}

	//Closes the FILE pointer.
  	fclose(fileP);

  	//If the line still exists, frees memory of line.
 	if (line)
    	free(line);

    //Returns 0.
  	return 0;
}