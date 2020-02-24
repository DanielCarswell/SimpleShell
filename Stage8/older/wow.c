void replace_alias(char** commands, char* originalValue)
{
	int placedTokens = 0;
	int pos = 1;
	int buffer = 100;
	char** newCommand = parseInput(originalValue);

	while(newCommand[placedTokens] != NULL)
		placedTokens++;

	if(commands[1] != NULL)
	{
		printf("%d", placedTokens);

		while(commands[pos] != NULL)
		{
			if(pos>=buffer) {
				buffer += 100;
				newCommand = realloc(newCommand, buffer);
			}
			strcpy(newCommand[placedTokens], commands[pos]);
			placedTokens++;
			pos++;
		}
	}
	printf("%d", placedTokens);
	int memPos = 0;
	while(commands[memPos] != NULL)
	{
		free(commands[memPos]);
		memPos++;	
	}
	free(commands);
	commands = NULL;

	commands = (char**) malloc(sizeof(char*) * buffer);
	printf("%d", placedTokens);

	int copyPos = 0;
	while(newCommand[copyPos] != NULL)
	{
		commands[copyPos] = (char*) malloc(sizeof(char) * strlen(newCommand[copyPos]));
		strcpy(commands[copyPos], newCommand[copyPos]);
		copyPos++;
	}
	printf("%d", placedTokens);

	memPos = 0;
	while(newCommand[memPos] == NULL)
	{
		free(newCommand[memPos]);
		memPos++;	
	}
	free(newCommand);
	newCommand = NULL;
	printf("%d", placedTokens);
}