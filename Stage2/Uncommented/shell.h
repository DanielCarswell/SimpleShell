/*Shell Header File*/

/*Shell.c*/
char* get_user_input(void);
char** parse_input(char*);

void startup_initialize(void);
void ctrlz_ignore(int);
void ctrlc_ignore(int);

/*Commands.c*/
void run_process(char**);
char* command_line(char**, int);