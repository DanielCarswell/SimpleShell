char* charpointertoarray(char** commands)
{
	int pos = 2;
	char* commandToAlias = malloc(1);
	while(commands[pos] != NULL)
	{
		int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos == 2)
			strcat(commandToAlias, commands[pos]);
		else
			strcat(strcat(commandToAlias, " "), commands[pos]);
		pos++;
	}

	return commandToAlias;
}

char* charpointertoarray2(char** commands)
{
	int pos = 0;
	char* commandToAlias = malloc(1);
	while(commands[pos] != NULL && commands[pos+1] != NULL )
	{
		strcpy(commandToAlias, "");
		printf("%s\n", commands[pos]);
		int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos!= 0)
			strcat(strcat(commandToAlias, " "), commands[pos]);
		else
			strcat(commandToAlias, commands[pos]);
		pos++;
	}
	return commandToAlias;
}