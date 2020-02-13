/*Shell Header File*/
int printTokens(char**);
void ctrlzIgnore(int);
void ctrlcIgnore(int);
char** parseInput(char*);
char* getUserInput(void);
int runProcess(char**);
void resetPaths(void);
int setpath(char**);
int getpath(char**);
int cd(char**);
int run_history(int,char**);
int element_in_history(char**);
void add_to_history(char**);
int print_history(char **);