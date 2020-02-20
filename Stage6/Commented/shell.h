/*Shell Header File*/

/*shell.c*/
char* getUserInput(void);
char** parseInput(char*);
void ctrlzIgnore(int);
void ctrlcIgnore(int);
void resetPaths(void);

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