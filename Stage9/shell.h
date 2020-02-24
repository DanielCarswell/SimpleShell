/*Shell Header File*/

/*shell.c*/
char* getUserInput(void);
char** parseInput(char*);
void resetPaths(void);
int internalCommandsCount(void);
void ctrlzIgnore(int);
void ctrlcIgnore(int);

/*commands.c*/
int printTokens(char**);
int runProcess(char**);
int setpath(char**);
int getpath(char**);
int cd(char**);

/*history.c*/
int run_history(int);
int element_in_history(char**);
int add_to_history(char[]);
int print_history(char**);
int save_history(void);
int load_history(void);

/*alias.c*/
int check_alias(char**);
int add_alias(char**);
int alias_count(void);
void run_alias(char**, char*);
int check_external_commands(char[]);
char* charpointertoarray(char**, int);
int save_aliases(void);
int load_aliases(void);
int recursive_check_alias(char**, char*);