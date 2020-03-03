#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//Definition for reused value, 512 is input limit.
#define Input_Max 512

//External global variables as necessary, check shell.c comments for more details on what
//the global variables are for.
extern char* InitialPathEnv;
extern char* current_history[];
extern char* internal_commands[];
extern void (*internal_functions[]) (char**);

/*
	This method exists as a sort of gateway so to speak for command, it checks if the command
	is a history invocation, an alias call, or an internal command, then runs functions/methods appropriately
	otherwise it will proceed to call the run_process method for executing command.

	Note: This command is called numerous times throughout executing a command in such cases as an alias being
	for a history invocation, or an alias of an alias of an alias and etc.
*/
void choose_process(char** commands)
{
	//If the first character of the first token is '!' then runs if statement.
	if(strncmp(commands[0], "!", 1) == 0)
	{
		//Calls a check to find if element exists in history.
		int check = element_in_history(commands);

		//If 1 was returned, returns to main method.
		if(check == 1)
			return;
	}

	//Calls check alias function, which returns 1 if alias was found.
	int check = check_alias(commands);

	//If 1 was returned, returns to main method.
	if(check == 1)
		return;

	//Loops for all internal commands.
	for(int i = 0; i < internal_commands_count(); i++)
	{
		//If the first token is equal to value at position i of internal commands,
		//then an internal command is being used.
		if(strcmp(commands[0], internal_commands[i]) == 0)
		{
			//Runs necessary function in position i of array function pointers, passing in commands.
			(*internal_functions[i])(commands);

			//Returns to main method.
			return;
		}
	}

	//Simply calls the run_process method passing a char**.
	run_process(commands);
}


/*
	This is the primary method of the the system, this method is used to run external commands.
	It works by using a fork to create a child process and adult process, the adult process waits
	for the child process to finish and the child process attempts to execute command.

	Note: Child process is used to stop something going wrong, if a problem occured in the adult process,
	it could potentially cause a 'zombie' process which would run in the background unneccesarily.
*/
void run_process(char** commands)
{
	//Initializing local variables, and allocating memory to temp (More will be allocated to temp later).
	char* temp = malloc(1);
	pid_t c_pid, pid;
	int status;

	//Copies value returned by command_line with allocated memory to temp.
	strcpy(temp, command_line(commands, 0));

	//Takes a new line for cleaner display.
	printf("\n");

	//Sets the c_pid value to 0 and higher seperately in child and parent process. 
	c_pid = fork();

	//If -1 was returned by fork, then print error and exit on status 1 because fork failed.
	if(c_pid == -1) {
		perror("Fork Failed");
		_exit(1);
	}

	//If 0 was returned then running the child process.
	if(c_pid == 0) {

		//Attempts to execute command entered by user, prints error with message showing entered command if fails.
		execvp(commands[0], commands);
		perror(temp);

		//Exit on status 1, this will only exit child process not the program.
		_exit(1);
	} 

	//If a value greater than 0 was returned, then in the parent process.
	else if (c_pid > 0) {
		
		//Waits for the child process to finish, if it fails to wait prints error and exits on status 1.
		if ((pid = wait(&status)) < 0) {
			perror("Wait Failed");
			_exit(1);
		}
	}

	//Freeing memory allocated to temp variable.
	free(temp);
}

/*
	Internal command: cd <parameter>

	This command is used to change the current working directory, on startup it is set to the
	same directory as the 'HOME' enviroment.
*/
void cd(char** commands)
{
	//Initialize local variable with small amount of memory allocation (more allocated later).
	char* temp = malloc(1);

	//If the 2nd token is not null meaning a directory change was entered, then sets temp to
	//full path change attempted.
	if(commands[1] != NULL)
		strcpy(temp, command_line(commands, 1));
	else
	{
		//Else, meaning if did not run, print appropriate message and return.
		printf("No path change entered\n");
		return;
	}

	//If a 3rd token was found, print an appropriate message.
	if(commands[2] != NULL)
		printf("cd only accepts one parameter\n");
	else if(strcmp(commands[1], "~") == 0)
	{
		//If the 2nd token was simple the character '~' then set working directory back to 'HOME' environment.
		//If it fails, then prints appropriate error.
		if (chdir(getenv("HOME")) != 0)
			perror("Directory change to HOME failed");
	}
	else
		//Else, meaning if and else if were not invoked, attempts to change working directory to 2nd token
		//Prints command entered and appropriate error if fails.
		if (chdir(commands[1]) != 0)
			perror(temp);
	
	//Frees the memory allocated to temp variable.
    free(temp);
}

/*
	Internal command: getpath

	Returns the value in the 'PATH' environment if it runs successfully.
*/
void get_path(char** commands)
{
	//Whilst there was no other tokens entered, so just 'getpath'.
	if(commands[1] == NULL)
		//Prints 'PATH' environment.
		printf("%s\n", getenv("PATH"));
	else
		//else prints error message that too many parameters were given to method.
		printf("No parameters allowed for getpath\n");
}

/*
	Internal command: setpath <parameter>

	changes the 'PATH' environment to parameter passed to command.
*/
void set_path(char** commands)
{
	//if the 2nd token is null, then no appropriate parameter passed.
	if(commands[1] == NULL)
	{
		//Prints message for user, then returns.
		printf("No path specified\n");
		return;
	}
	else if(commands[2] != NULL)
	{
		//If a 2nd parameter existed, print an appropriate message and return.
		printf("Too many arguments, setpath only takes 1 parameter\n");
		return;
	}

	//If the user input as parameter the character '~', resets PATH to what it was on startup.
	if(strcmp(commands[1], "~") == 0) {
		if(setenv("PATH", InitialPathEnv, 1) == -1)
			//Prints an error if fails.
			perror("Failed to reset environment PATH");

		//returns to choose_process method,
		return;
	}

	//Attempts to change 'PATH' to entered parameter, if fails prints appropriate error message.
	if(setenv("PATH", commands[1], 1) == -1)
		perror("Failed to change environment PATH");
}

//Creates a char* string value of entered user tokens for given position and after.
// Mainly used for displaying the command that failed, or getting a value to store in aliases.
// e.g. command: alias dashl ls -l position 2: returns char* with value "ls -l"
char* command_line(char** commands, int position)
{
	//Localizing local variables and allocating memory to line.
	int pos = position;
	char* line = malloc(1);
	line = NULL;

	//Loops for call commands til a null is found for passed position and onwards.
	while(commands[pos] != NULL)
	{
		//Reallocating memory to line as necessary.
		line = realloc(line, (Input_Max * sizeof(char)));

		//If first position checked, set line to value at pos of commands.
		if(pos == position)
			sprintf(line, "%s", commands[pos]);
		else
			//Otherwise, overwrites the line with the line, a space and value at pos of commands.
			sprintf(line, "%s %s", line, commands[pos]);

		//Increment pos.
		pos++;
	}

	//Returns char*.
	return line;
}