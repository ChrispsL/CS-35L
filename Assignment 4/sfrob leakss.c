#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/*
qsort
int compare(const void * a, const void *b)
qsort(values, n, sizeof(int), compare)


Valgrind
dynamic analysis tool esp for memory leaks
valgrind --leak-check=full ./sfrob < foo.txt


frobcmp
takes char const * a, * b (points to array of non-space bytes)
returns negative a<b
zero a==b
positive a>b
Lexicographic comparison of unfrobnicated bytes
frobnication is bitwise XOR ^ with 42
read byte-by-byte (getchar reads until EOF)
sort records w/o decoding (qsort, frobcmp)
output in frob text (fprintf, putchar)
work on empty & large files too
cat ‘ksopgkpdr\gds’ > foo.txt	→	./sfrob < foo.txt	printf ‘aripjpr’ | ./sfrob
New line → frob → space


Tips for homework
exitl, not return for errors
1D vs 2D arrays?
Test output with od -c or -a
Thorough error checking, print appropriate message
Plug allllll memory leaks

*/

void releaseInput(char ** inp, int n)
{
	for(int i=0; i <= n; i++)
	{
		free(inp[i]);
	}
	free(inp);
}

void logErr(const char * message)
{
	fprintf(stderr, "Error: %s %d\n", message, errno);
	exit(1);
}
// int frobcmp(char const* a, char const* b)
// {
//     for ( ; *a == *b; a++, b++)
//         if (*a == ' ')
//             return 0;
//     return ((*a)^42 < (*b)^42) ? -1 : 1;
    
// }
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
	size_t maxLineSize=16, maxSize=16, lineLength=0, lineNum=0;	// in terms of # characters
	char currChar;
	char ** input = (char**) malloc(sizeof(char*)*maxSize);	// [i][] represents 1 line
	if(input==0) {
		releaseInput(input, lineNum);
		logErr("Couldn't init allocation");
	}
	input[0] = (char*) malloc(sizeof(char)*maxLineSize);	// [0][0~15] is 1st line
	if(input[0]==0) {
		releaseInput(input, lineNum);
		logErr("Couldn't init allocation");
	}

	while((currChar = getchar()) != EOF)
	{
		if(ferror(stdin))
		{
			releaseInput(input, lineNum);
			logErr("std input");
		}
		if(lineLength==0 && currChar==' ') continue;

		input[lineNum][lineLength] = currChar;
		lineLength++;	// there's stuff in the line
		if(currChar==' ')
		{
			while(lineLength < maxLineSize)
			{
				input[lineNum][lineLength++] = 0;	
				// null the rest of the row (line) so we don't run into trouble reading
			}

			if(lineNum >= maxSize)
			{
				maxSize *= 2;
				char** biggerInput = (char**) realloc(input, sizeof(char*) * maxSize);
				if(biggerInput==0)
				{
					releaseInput(input, lineNum);
					logErr("Reallocation of lines");
				}
				input = biggerInput;
			}
			// currLine = input[lineNum++];	// move to next row
			lineNum++;

			maxLineSize = 16;	// default new line length
			lineLength = 0;
			input[lineNum] = (char*) malloc(sizeof(char)*maxLineSize);
			if(input[lineNum]==0){
					releaseInput(input, lineNum);
					logErr("Malloc of new line");
			}
		}
		if(lineLength >= maxLineSize)
		{
			maxLineSize *= 2;
			char* biggerLine = (char*) realloc(input[lineNum], sizeof(char) * maxLineSize);
			if(biggerLine==0){
				releaseInput(input, lineNum);
				logErr("Reallocation of single line");
			}
			input[lineNum] = biggerLine;
		}
	}
	if(lineLength==0 && lineNum==0)	// all spaces or empty file
	{
		releaseInput(input, lineNum);
		return 0;
	}
	if(input[lineNum][lineLength-1] != ' ')
	{
		if(lineLength != maxLineSize)
			input[lineNum][lineLength++] = ' ';
	}
	while(lineLength < maxLineSize)
	{
		input[lineNum][lineLength++] = 0;	
		// null the rest of the row (line) so we don't run into trouble reading
	}




	lineNum++;	// lineNum corresponds to index, since it started at 0
	qsort(input, lineNum, sizeof(char*), &frobcmpWrapper);

	for (size_t i = 0; i < lineNum; i++)
    {
  		printf("%s", input[i]);
  		free(input[i]);
	}
	free(input);



	// releaseInput(input, lineNum);
}

int main()
{
	sfrob();
}