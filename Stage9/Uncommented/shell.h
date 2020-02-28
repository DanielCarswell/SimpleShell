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
void run_process(char**);
void choose_process(char**);
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

int save_history(void);
int load_history(void);

/*alias.c*/
void print_aliases(void);
int check_alias(char**);
int add_alias(char**);
int unalias(char**);
void run_alias(char**, char*);
int recursive_check_alias(char**, char*);
int infinite_alias_check(char*, char*);

int save_aliases(void);
int load_aliases(void);