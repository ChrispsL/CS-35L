#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
void logErr(const char * message)
{
	write(2, message, strlen(message));
	exit(1);
}

size_t ignoreCase;

int frobcmp(const char * a, const char * b)
{
	while(*a==*b)
	{
		if(*a == ' ')
			return 0;
		a++;
		b++;
	}
	if(*a==' ' || (char)((*a)^42) > (char)((*b)^42)) return 1;
	if(*b==' ' || (char)((*a)^42) < (char)((*b)^42)) return -1;
	return 0;
}

int frobcmpCase(const char * a, const char * b)
{
	while(toupper((*a)^42) == toupper((*b)^42))
	{
		if(*a == ' ')
			return 0;
		a++;
		b++;
	}
	if(*a==' ' || toupper((*a)^42) > toupper((*b)^42)) return 1;
	if(*b==' ' || toupper((*a)^42) > toupper((*b)^42)) return -1;
	return 0;
}

int frobcmpWrapper(const void * a, const void * b)
{
	return frobcmp(*(const char **) a, *(const char**) b);
}

int frobcmpCaseWrapper(const void * a, const void * b)
{
	return frobcmpCase(*(const char **) a, *(const char**) b);
}

int sfrob(size_t ignoreCase)
{
	struct stat buffer;
	if(fstat(0, &buffer) < 0)
	{
		logErr("File input error\n");
	}
	char readIn[2];
	size_t readLen=1, currLength=0, numWords=0, fileSize=buffer.st_size+1;

	char currChar, prevChar = ' ';
	char* input = (char*) malloc(sizeof(char)*fileSize);
	// check malloc error

	while(readLen != 0)
	{
		readLen = read(0, readIn, 1);
		if(readLen < 0)
		{
			free(input);
			logErr("Error trying to read stdin\n");
		}
		if(readLen==0) break;
		currChar = readIn[0];

		if(prevChar==' ' && currChar==' ') continue;
		prevChar = currChar;

		input[currLength] = currChar;
		currLength++;
		if(currChar==' ')
		{
			numWords++;
		}
		if(currLength >= fileSize)
		{
			fileSize *= 2;
			char* biggerInput = (char*) realloc(input, sizeof(char)*fileSize);
			if(biggerInput==0)
			{
				free(input);
				logErr("Couldn't reallocate\n");
			}
			input = biggerInput;
		}
	}
	if(currLength==0)	// all spaces or empty file
	{
		free(input);
		return 0;
	}
	if(input[currLength-1] != ' ')
	{
		if(currLength != fileSize)
		{
			input[currLength++] = ' ';
			numWords++;
		}
		else
		{
			fileSize *= 2;
			char* biggerInput = (char*) realloc(input, sizeof(char)*fileSize);
			if(biggerInput==0)
			{
				free(input);
				logErr("Couldn't reallocate\n");
			}
			input = biggerInput;
			input[currLength++] = ' ';
			numWords++;
		}
	}

	char** wordList = (char**) malloc(sizeof(char*)*numWords); 
	if(wordList==0)
	{
		free(wordList);
		free(input);
		logErr("Couldn't allocate words");
	}
	char* curr = input;
	size_t count = 0;
    for (int i = 0; i < currLength; i++)
    {
        if (input[i] == ' ')
        {
            wordList[count++] = curr;
            curr = input + i + 1;
        }
    }

    int (* func)(const void*, const void*);
    if(ignoreCase) func = &frobcmpCaseWrapper;
    else func = &frobcmpWrapper;
	qsort(wordList, numWords, sizeof(char*), func);

	for(int i = 0; i < count; i++)
	{
		char* word = wordList[i];
		while(1)
		{
			write(1, word, 1);
			// check write error
			if(*word == ' ') break;
			word++;
		}
	}
	//releaseInput(&wordList, numWords);
	free(input);
	free(wordList);
}


int main(int argNum, char* args[])
{
	if(argNum==1)
		sfrob(0);
	else if(argNum==2 && args[1][1]=='f')
	{
		sfrob(1);
	}
	else {
		if(argNum < 1 || argNum > 2)
			logErr("Invalid number of arguments\n");
		else
			logErr("Invalid argument\n");
	}
}