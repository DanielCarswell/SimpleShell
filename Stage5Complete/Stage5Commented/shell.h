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
int run_history(int);
int element_in_history(char**);
