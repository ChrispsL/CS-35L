#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argNum, const char* args[])
{
	if(argNum != 3)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		exit(1);
	}
	const char* from = args[1];
	const char* to = args[2];
	size_t transSize = strlen(from);
	if(transSize != strlen(to))
	{
		fprintf(stderr, "Argument lengths not equal\n");
		exit(1);
	}

	for(int i = 0; i < transSize; i++)
	{
		for(int j = 0; j < i; j++)
		{
			if(from[j] == from[i])
			{
				fprintf(stderr, "Duplicate character in first argument\n");
				exit(1);
			}
		}
	}

	char currChar;
	while(1)
	{
		currChar = getchar();
		if(feof(stdin)) break;
		if(ferror(stdin))
		{
			fprintf(stderr, "stdin input error\n");
			exit(1);
		}
		for(int i = 0; i < transSize; i++)
		{
			if(currChar == from[i])
			{
				currChar = to[i];
				break;
			}
		}
		putchar(currChar);
	}
	return 0;
}
