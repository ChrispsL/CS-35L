#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file = open("input.txt", O_WRONLY | O_APPEND);
	for(int i = 0; i < 5000000; i++)
	{
		write(file, "a", 1);
	}
	return 0;
}