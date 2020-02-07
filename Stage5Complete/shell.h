/*Shell Header File*/
int cd(char**);
int element_in_history(char**);
int getpath(char**);
int setpath(char**);
int run_history(int);
char* getUserInput(void);
int printTokens(char**);
int runProcess(char**);
char** parseInput(char*);
void ctrlzIgnore(int);
void ctrlcIgnore(int);
void resetPaths(void);