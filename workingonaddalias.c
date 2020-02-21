int add_alias(char** commands)
{
	char temp[Input_Max];
	int index = 0;

	for(int i = 0; i < 3; i ++)
		if(commands[i] == NULL)
		{
			printf("Not enough arguments\n");
			return 0;
		}

	while(command_aliases[index][0] != NULL)
	{
		if(strcmp(command_aliases[index][0], commands[2]) == 0)
		{
			printf("Old alias overwritten with: %s\n", commands[1]);
			sprintf(temp, commands[1]);
			free(command_aliases[index][1]);
			command_aliases[index][1] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
			strcpy(command_aliases[index][1], temp);
			return 0;
		}
		index++;
	}

	for(int i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			if(commands[3] != NULL)
			{
				char* commandTemp;
				commandTemp = charpointertoarray(commands, 0);
				sprintf(temp, commandTemp);	
			}
			else
			sprintf(temp, commands[2]);
		}
		else
			sprintf(temp, commands[1]);

		command_aliases[index][i] = (char *) malloc(sizeof(char) * (strlen(temp)+1));
		strcpy(command_aliases[index][i], temp);
	}
	return 0;
}

char* charpointertoarray(char** commands, int choice)
{
	int pos = 2;
	char* commandToAlias = malloc(1);
	if(choice == 0)
	while(commands[pos] != NULL)
	{
		strcpy(commandToAlias, "");
		int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
		commandToAlias = realloc(commandToAlias, buffer);
		if(pos == 2)
			strcat(commandToAlias, commands[pos]);
		else
			strcat(strcat(commandToAlias, " "), commands[pos]);
		pos++;
	}
	else if(choice == 1)
	{
		pos = 0;
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
	}
	else
	{
		while(commands[pos] != NULL && commands[pos+1] != NULL )
		{
			strcpy(commandToAlias, "");
			printf("%s\n", commands[pos]);
			int buffer = strlen(commands[pos]+2) + strlen(commandToAlias);
			commandToAlias = realloc(commandToAlias, buffer);
			if(pos != 0)
				strcat(strcat(commandToAlias, " "), commands[pos]);
			else
				strcat(commandToAlias, commands[pos]);
			pos++;
		}
	}
	return commandToAlias;
}