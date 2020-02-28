/*Shell Header File*/

/*Shell.c*/
char* get_user_input(void);
char** parse_input(char*);
int internal_commands_count(void);

void startup_initialize(void);
void exit_program(void);

void ctrlz_ignore(int);
void ctrlc_ignore(int);

/*Commands.c*/
void choose_process(char**);
void run_process(char**);
char* command_line(char**, int);

int set_path(char**);
int get_path(char**);
int cd(char**);

/*History.c*/
int run_history(int);
int element_in_history(char**);
void positive_history(char**);
void negative_history(char**);
void add_to_history(char[]);
int print_history(char**);