#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int fd;
	if ((fd = open("ps_out", O_CREAT|O_WRONLY|O_TRUNC)) == -1)
	{
		perror("open");
		exit(1);
	}

	dup2(fd, STDOUT_FILENO);	//精髓

	execlp("ps", "ps", "aux", NULL);
	perror("execlp error");

	return 0;
}
