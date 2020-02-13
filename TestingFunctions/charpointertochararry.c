#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char temp[50];
char* history[99];

int add2array(char*, int);

int main(void)
{
    int i = add2array("aw doawkwoa kaw", 0);
    printf("%s", history[0]);
	free(s1[0]);
	free(*s1);
	return i;
}

int add_to_history(char *a, int b)
{
	history[b] = (char *) malloc(sizeof(char) * (strlen(a)+1));
    printf("add2array...\n");
    sprintf(temp,a);
    strcpy(history[b],temp);
    return 0;
}
