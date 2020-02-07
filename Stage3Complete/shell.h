/*Shell Header File*/
int getpath(char**);
int setpath(char**);
char* getUserInput(void);
int printTokens(char**);
int runProcess(char**);
char** parseInput(char*);
void ctrlzIgnore(int);
void ctrlcIgnore(int);
void resetPaths(void);