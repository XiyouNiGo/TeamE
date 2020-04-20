#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int ret;
	long int read_max_input, write_max_input;
	int fd[2];

	if ((ret = pipe(fd)) == -1)
	{
		perror("pipe error");
		exit(1);
	}

	read_max_input = fpathconf(fd[0], _PC_MAX_INPUT);
	write_max_input = fpathconf(fd[1], _PC_MAX_INPUT);

	printf("read_max_input is %ld\n", read_max_input);
	printf("write_max_input is %ld\n", write_max_input);

	return 0;
}
