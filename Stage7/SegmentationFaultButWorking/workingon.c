void replace_alias(char** commands, char* originalValue)
{
	int placedTokens = 0;
	int pos = 0;
	int buffer = 100;
	char** newCommand = parseInput(originalValue);

	if(pos>=buffer) {
			buffer += 100;
			tokens = realloc(tokens, buffer);
		}

	while(newCommand[pos] != NULL)
	{
		pos++;
	}

	placedTokens = pos;
	pos = 1;
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

	int memPos = 0;
	while(commands[memPos] != NULL)
	{
		free(commands[memPos]);
		memPos++;	
	}
	free(commands);
	commands = NULL;

	commands = (char**) malloc(sizeof(char) * buffer);

	int copyPos = 0;
	while(newCommand[copyPos] != NULL)
	{
		commands[copyPos] = (char*) malloc(sizeof(char) * strlen(newCommand[copyPos]));
		strcpy(commands[copyPos], newCommand[copyPos]);
		copyPos++;
	}

	add_to_history(commands[0]);

	memPos = 0;
	while(newCommand[memPos] == NULL)
	{
		free(newCommand[memPos]);
		memPos++;	
	}
	free(newCommand);
	newCommand = NULL;
}


void replace_alias(char** commands, char* originalValue)
{
	int placedTokens = 0;
	int pos = 0;
	int buffer = 100;
	char** newCommand = parseInput(originalValue);

	while(newCommand[pos] != NULL)
	{
		pos++;
	}

	placedTokens = pos;
	pos = 1;
	printf("%d", placedTokens);

	while(commands[pos] != NULL)
	{
		newCommand[placedTokens] = (char*) malloc(sizeof(check_external_commands)*20);
		strcpy(newCommand[placedTokens], commands[pos]);
		placedTokens++;
		pos++;
	}

	pos = 0;
	while(commands[pos] != NULL)
	{
		free(commands[pos]);
		pos++;
	}

	int memPos = 0;
	while(commands[memPos] == NULL)
	{
		free(commands[memPos]);
		memPos++;	
	}
	free(commands);
	commands = NULL;

	commands = (char**) malloc(sizeof(char*) * buffer);

	int copyPos = 0;
	while(newCommand[copyPos] != NULL)
	{
		commands[copyPos] = (char*) malloc(sizeof(char) * 50);
		strcpy(commands[copyPos], newCommand[copyPos]);
		copyPos++;
	}

	memPos = 0;
	while(newCommand[memPos] == NULL)
	{
		free(newCommand[memPos]);
		memPos++;	
	}
	free(newCommand);
	newCommand = NULL;
}