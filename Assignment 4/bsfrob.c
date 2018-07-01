#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// void releaseInput(char *** inp, int n)
// {
// 	for(int i=0; i < n; i++)
// 	{
// 		free(*inp[i]);
// 	}
// 	free(*inp);
// }

void logErr(const char * message)
{
	fprintf(stderr, "Error: %s %d\n", message, errno);
	exit(1);
}

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

int frobcmpWrapper(const void * a, const void * b)
{
	return frobcmp(*(const char **) a, *(const char**) b);
}

int sfrob()
{
	long currMax = 16, currLength = 0, numWords=0;
	char currChar, prevChar = ' ';
	char* input = (char*) malloc(sizeof(char)*currMax);

	while((currChar = getchar()) != EOF)
	{
		if(ferror(stdin))
		{
			free(input);
			logErr("std input");
		}
		if(prevChar==' ' && currChar==' ') continue;
		prevChar = currChar;

		input[currLength] = currChar;
		currLength++;
		if(currChar==' ')
		{
			numWords++;
			// if(numWords >= wordsSize)
			// {
			// 	wordsSize *= 2;
			// 	char** biggerWords = (char**) malloc(sizeof(char*)*wordsSize);
			// 	if(biggerWords==0)
			// 	{
			// 		//free
			// 	}
			// 	words = biggerWords;
			// }
			// words[numWords++] = input;
			// printf("%s\n", input);
			// currLength = 0;
			// for(int i = 0; i < currMax; i++)
			// 	input[i] = 0;
		}
		if(currLength >= currMax)
		{
			currMax *= 2;
			char* biggerInput = (char*) realloc(input, sizeof(char)*currMax);
			if(biggerInput==0)
			{
				free(input);
				logErr("Couldn't reallocate");
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
		if(currLength != currMax)
		{
			input[currLength++] = ' ';
			numWords++;
		}
		else
		{
			currMax *= 2;
			char* biggerInput = (char*) realloc(input, sizeof(char)*currMax);
			if(biggerInput==0)
			{
				free(input);
				logErr("Couldn't reallocate");
			}
			input = biggerInput;
			input[currLength++] = ' ';
			numWords++;
		}
	}

	// size_t maxWords = 4;
	// char** wordList = (char**) malloc(sizeof(char*)*maxWords);
	// char* curr = input;
	// size_t i, count = 0, words = 0;
	// for(i = 0; i < currLength; i++)
	// {
	// 	if(curr[i] != ' ') count++;
	// 	else {
	// 		wordList[words] = (char*) malloc(sizeof(char)*count);
	// 		for(int j = 0; j <= count; j++)
	// 			wordList[words][j] = curr[j];
	// 		curr = input + (i + 1);
	// 		printf("%s\n", curr);
	// 		count = 0;
	// 		words++;
	// 		if(words >= maxWords)
	// 		{
	// 			maxWords *= 2;
	// 			char** biggerWords = (char**) realloc(wordList, sizeof(char*)*maxWords);
	// 			if(biggerWords==0)
	// 			{
	// 				//free
	// 			}
	// 			wordList = biggerWords;
	// 			printf("realloc");
	// 		}
	// 	}
	// }

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

	qsort(wordList, numWords, sizeof(char*), &frobcmpWrapper);

	for(int i = 0; i < count; i++)
	{
		char* word = wordList[i];
		while(1)
		{
			printf("%c", *word);
			if(*word == ' ') break;
			word++;
		}
	}

	//releaseInput(&wordList, numWords);
	free(input);
	free(wordList);
}


int main()
{
	sfrob();
}