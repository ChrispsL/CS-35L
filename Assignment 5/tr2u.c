#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argNum, const char* args[])
{
	if(argNum != 3)
	{
		write(2, "Invalid number of arguments\n", 28);
		exit(1);
	}
	const char* from = args[1];
	const char* to = args[2];
	size_t transSize = strlen(from);
	if(transSize != strlen(to))
	{
		write(2, "Argument lengths not equal\n", 27);
		exit(1);
	}

	for(int i = 0; i < transSize; i++)
	{
		for(int j = 0; j < i; j++)
		{
			if(from[j] == from[i])
			{
				write(2, "Duplicate character in first argument\n", 37);
				exit(1);
			}
		}
	}

	char input[2];
	size_t readLen;
	while(readLen != 0)
	{
		readLen = read(0, input, 1);
		if(readLen < 0)
		{
			write(2, "Input error\n", 12);
			exit(1);
		}
		if(readLen==0)
			break;
		for(int i = 0; i < transSize; i++)
		{
			if(input[0] == from[i])
			{
				input[0] = to[i];
				break;
			}
		}
		if(write(1, input, 1) < 0)
		{
			write(2, "Output error\n", 13);
			exit(1);
		}

		// readLen = read(0, input, 1);
	}
	return 0;
}