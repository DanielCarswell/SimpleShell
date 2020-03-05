#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

//Definitions for reused values, 512 is input limit, and Delimiter is for tokenizing.
#define Input_Max 512
#define Delimiter " \n\t;&><|"


//Global strings, memory allocated on startup and free'd on exitting.
char* loopstopper;
char* InitialHomeEnv;
char* InitialPathEnv;

//Global variables for storing history elements and aliases, used across files
//using them as globals highly limits complexity, though with more time could be implemented otherwise.
char* current_history[999];
char* command_aliases[10][2];


//List of internal command names, compares first token entered by user to these
//in order to confirm if an internal command is being invoked.
char* internal_commands[] = {
  "getpath",
  "setpath",
  "cd",
  "history",
  "alias",
  "unalias"
};

//Array of function pointers that works very well with the internal commands list above,
//uses the position to associate the necessary function in order to achieve a cleaner function call.
void (*internal_functions[]) (char **) = {
  &get_path,
  &set_path,
  &cd,
  &print_history,
  &add_alias,
  &unalias
};


//Main, calls startup method to initialize data such as history and alias load, then loops
//taking in commands and attempting to run them til the user chooses to exit the shell.
int main(void)
{
	//Calls method to initialize globals and load persistent history/ aliases.
	startup_initialize();

	//Declaring local variables used inside this method.
	char* line;
	char* token;
	char** tokens;
	char temp[Input_Max];
	int addHistCheck = 0;

	//Will loop infinitely, however appropriate exits are achieved where necessary that 
	//are not affected by this definition of an infinite while loop.
	while(1) {

		//Gets entered user line from function with char* return type.
		line = get_user_input();

		//Stores the current line in the temp variable.
		sprintf(temp, "%s", line);

		//Creates a token using strtok to be parsed.
		token = strtok(line, Delimiter);

		//Adds history to history if called, otherwise should add after running.
		if(strcmp(token, "history") == 0)
		{
			add_to_history(temp);
			addHistCheck = 1;
		}
		
		//Whilst the token is not null, runs if.
		if (token) {
			//Calls function to parse char* token into type char**.
			tokens = parse_input(token);

			//Calls the choose_process method which will figure out what to do with the command.
			choose_process(tokens);

			//Freeing the current memory allocation of tokens as it will be reassigned memory.
			free(tokens);
  		}

  		//Whilst a command history was not implicitly called, attempt to add temp as a history element.
  		if(line[0] != '!' && addHistCheck == 0)
			add_to_history(temp);

		//Frees memory allocation of line, as it will be reallocated memory.
		free(line);	

		//Reset variable to 0.
		addHistCheck = 0;
	}

	//Returns 0 closing the program, however it exits elsewhere and should not hit this line.
	return 0;
}


//Simply allocates memory to the line, and gets users input and returns char*.
char* get_user_input(void)
{
	//Allocating memory equal to the size of a char times the maximum input.
	char *line = (char *) malloc(sizeof(char) * Input_Max);

	//Simple display prompt for user with line indentation for a cleaner text line.
	printf("\n> ");

	/*Attempts to get user input, if it returns null then runs exit_program function as
	<Ctrl>-d has been entered by the user in such case.*/
	if(fgets(line, Input_Max, stdin) == NULL)
		exit_program();

	//Returns local variable of type char*.
	return line;
}

/*
	This method takes in a char* that as been tokenized, what it does is it will first allocate
	memory to a local variable tokens, it will ensure that the users first token is not an exit
	command in which case it will exit. Then it will loop whilst a token exists and add that in a 
	new position of the local variable tokens, using a buffer to reallocate memory if necessary
	before returning the double char pointer.
*/
char** parse_input(char* token)
{
	//Initialising local variables.
	int pos = 0;
	int buffer = 100;

	//Initialising local variable using memory allocation.
	char** tokens = malloc(buffer * sizeof(char*));

	//If first token is equal to exit, then runs exit_program method.
	if(strcmp(token, "exit") == 0)
		exit_program();


	//Loops whilst token is not null, meaning another token exists.
	while(token)
	{
		//Sets current position of tokens equal to the current token.
		tokens[pos] = token;

		//Increments pos variable.
		pos++;

		//Checks if more memory allocation is needed.
		if(pos >= buffer) {
			//Increases the size of the buffer and then allocates the necessary memory.
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

		//Gets next token before loop attempts to re-execute.
		token = strtok(NULL, Delimiter);
	}

	//Clears up any old tokens that existed in positions beyond the current point,
	//This is necessary for example if you entered: ps aux then ls, this would happen: ls aux 
	while(tokens[pos] != NULL)
	{
		tokens[pos] = NULL;
		pos++;
	}

	//Returns char** of tokens.
	return tokens;
}

//At the beginning of the program, initializes necessary directory, and global variables.
//Such as history and aliases, and signal blockers, as well as initializing working directory.
void startup_initialize(void)
{
	//Initializes signal blocks for <ctrl-c> and <ctrl-z>.
	signal(SIGTSTP, ctrlz_ignore);
	signal(SIGINT, ctrlc_ignore);

	//Initialize memory allocation for global environment variables.
	InitialHomeEnv = (char*) malloc(sizeof(char) * strlen(getenv("HOME"))+1);
	InitialPathEnv = (char*) malloc(sizeof(char) * strlen(getenv("PATH"))+1);

	//Copy the HOME and PATH environments to global variables for resetting on exit.
	strcpy(InitialHomeEnv, getenv("HOME"));
	strcpy(InitialPathEnv, getenv("PATH"));

	//Attempts to change the working directory to the HOME directory.
	if (chdir(getenv("HOME")) != 0) {

		//Prints error if necessary.
		perror("Directory change failed");
	}

	//Attempts to load history from .hist_list file, will print error message if fails.
	if(load_history() == -1)
		printf("Failed to load history\n");
	
	//Attempts to load aliases from .alias_list file, will print error message if fails.
	if(load_aliases() == -1)
		printf("Failed to load aliases\n");

	//Initializes memory allocation for global variable loopstopper.
	loopstopper = malloc(sizeof(char) * Input_Max);
}

//Called when user enters 'exit' or enters '<ctrl>-d', resets the working directory to HOME,
//as well as resetting the HOME and PATH variables, then saves aliases and historys and finishes up
//by freeing all the memory allocation for global variables where necessary.
void exit_program(void)
{
	//Initializing local variables.
	int pos = 0;
	
	//Attempts to reset HOME environment using global variable, prints error if fails.
	if(setenv("HOME", InitialHomeEnv, 1) == -1)
		perror("Environment reset failed");

	//Attempts to reset PATH environment using global variable, prints error if fails.
	if(setenv("PATH", InitialPathEnv, 1) == -1)
		perror("Environment reset failed");

	//Attempts to reset the working directory to HOME environment, prints error if fails.
	if(chdir(getenv("HOME")) != 0)
		perror("Directory change failed");

	//Attempts to save history elements to .hist_list file, prints error if fails.
	if(save_history() == -1)
		printf("Failed to save history.");

	//Attempts to save aliases to .alias_list file, prints error if fails.
	if(save_aliases() == -1)
		printf("Failed to save aliases.");

	//Loops while position of pos in current_history is not null, hence another history element exists.
	while(current_history[pos] != NULL)
	{
		//Frees memory allocation in pos of current_history.
		free(current_history[pos]);

		//Increments pos.
		pos++;
	}

	//Set pos back to 0.
	pos = 0;

	//Loops while position of pos in command_aliases is not null, hence another alias element exists.
	while(command_aliases[pos][0] != NULL)
	{
		//Frees memory allocation in pos of command_aliases.
		free(command_aliases[pos][0]);
		free(command_aliases[pos][1]);

		//Increments pos.
		pos++;
	}

	//Freeing other memory allocated global variables.
	free(loopstopper);
	free(InitialHomeEnv);
	free(InitialPathEnv);

	//Taking a new line, before exitting on exit status 1.
	printf("\n");
	_exit(1);
}

//Returns an integer of the number of inbuilt commands from the hard coded global variable internal_commands.
int internal_commands_count(void)
{
	//Initializing local variables.
	int size = 0;

	//Increments size whilst command in position equal to size is not null.
	while(internal_commands[size] != NULL)
	{
		size++;
	}

	//Returns int, equal to number of internal commands that exist.
	return size;
}

//Used to redirect signal of <ctrl>-z, this will stop it crashing program.
void ctrlz_ignore(int sig_num) 
{ 
	//Reinitializes the signal to call this method instead of running normally.
	signal(SIGTSTP, ctrlz_ignore);
}

//Used to redirect signal of <ctrl>-c, this will stop it crashing program.
void ctrlc_ignore(int sig_num) 
{
	//Reinitializes the signal to call this method instead of running normally.
	signal(SIGINT, ctrlc_ignore);
} 