/*






	int x,y,z, check;
	char* numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

		if(strncmp(&commands[0][1],"\0",1) == 0)
			printf("No element of history selected\n");
		else if(strncmp(&commands[0][1],"!",1) == 0)
			check = run_history(0);

	if(strncmp(&commands[0][4], "\0", 1) != 0)
	{
		printf("Element out of bounds\n");
		return 1;
	}
	else if(strncmp(&commands[0][3], "\0", 1) != 0)
	{
		for(x = 0; x < 10; x++)
			for(y = 0; y < 10; y++)
				for(z = 0; z < 10; z++)
					if(strncmp(&commands[0][1], numbers[x], 1) == 0)
						if(strncmp(&commands[0][2], numbers[y], 1) == 0)
							if(strncmp(&commands[0][3], numbers[z], 1) == 0)
								check = run_history((x*100) + (y*10) + z);
	}
	else if(strncmp(&commands[0][2], "\0", 1) != 0)
	{
		for(y = 0; y < 10; y++)
			for(z = 0; z < 10; z++)
				if(strncmp(&commands[0][2], numbers[y], 1) == 0)
					if(strncmp(&commands[0][3], numbers[z], 1) == 0)
						check = run_history((y*10) + z);
	}
	else
	{
		for(z = 0; z < 10; z++)
			if(strncmp(&commands[0][3], numbers[z], 1) == 0)
				check = run_history(z);
	}

	return check;*/







int save_history(void)
{
  int pos = 0;
  FILE* fPointer;
  char holdValue[Input_Max];
  char concat[15];
  strcpy(concat, "\n");
  fPointer = fopen(".hist_list", "w");
  
  if(fPointer == NULL) return -1;

  while(current_history[pos] != NULL){
    strcpy(holdValue, current_history[pos]);  
    strcat(holdValue, concat);
    fputs(holdValue, fPointer);
    pos++;
  }

  fclose(fPointer);
  return 0;
}

int load_history(void)
{
  	int i = 0;
  	FILE * fPointer;
  	char* strings[Input_Max];
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;

  	fPointer = fopen(".hist_list", "r");
  	if (fPointer == NULL) return -1;

  	while ((read = getline(&line, &len, fPointer)) != EOF) 
  	{
    	strings[i] = malloc(100);
    	line[strlen(line)-1] = '\0';
    	strcpy(strings[i++], line);
  	}

  	for(int x = 0; x < i-1; x++)
  	{
    	add_to_history(strings[x]);
    	free(strings[x]);
    }

    free(*strings);
  	fclose(fPointer);
  	if (line)
   		free(line);
  	return 0;
}




int check = 0;
	char* chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	//Uses strncmp for comparing an nth element, instead of strcmp
	//which compares a full string.
	if(strncmp(commands[0], "!", 1) == 0)
	{
		if(strncmp(&commands[0][1],"\0",1) == 0)
			printf("No element of history selected\n");
		else if(strncmp(&commands[0][1],"!",1) == 0)
			check = run_history(0);
		else
			for(int i = 0; i < 10; i++)
			{
				if(strncmp(&commands[0][1], chars[i], 1) == 0)
					if(strncmp(&commands[0][2], "\0", 1) == 0)
						check = run_history(i);
					else
						for(int j = 0; j < 10; j++)
						{
							if(strncmp(&commands[0][2], chars[j], 1) == 0)
								if(strncmp(&commands[0][3], "\0", 1) == 0)
								{
									check = run_history((i*10) + j);
									return check;
								}
						
							if(strncmp(&commands[0][3], "\0", 1) == 0)
							{
								check = run_history((i*10) + j);
								return check;
							}
							else
								for(int x = 0; x < 10; x++)
								{
									if(strncmp(&commands[0][3], chars[x], 1) == 0)
									{
										if(strncmp(&commands[0][4], "\0", 1) == 0)
										{
											check = run_history((x*100) + (i*10) + j);
											return check;
										}
										else
										{
											printf("No element in history\n");
											return 1;
										}
									}
								}
						}
				else if(i == 9 && check == 0 && strncmp(&commands[0][3], "\0", 1) == 0)
				{
					printf("Invalid character after !\n");
					break;
				}
			}
	}

	return check;