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

void set_path(char**);
void get_path(char**);
void cd(char**);

/*History.c*/
int run_history(int);
int element_in_history(char**);
void positive_history(char**);
void negative_history(char**);
void add_to_history(char[]);
void print_history(char**);

int save_history(void);
int load_history(void);