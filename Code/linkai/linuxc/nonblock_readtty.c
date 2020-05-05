#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char buf[10];
	int fd, ret;

	if ((fd = open("/dev/tty", O_RDONLY|O_NONBLOCK)) < 0)
	{
		perror("open /dev/tty");
		exit(1);
	}
	while (1)
	{
		if ((ret = read(fd, buf, 10)) < 0)
		{
			if (errno != EAGAIN)
			{
				perror("read /dev/tty");
				exit(1);
			}
			else
			{
				write(STDOUT_FILENO, "try again\n", strlen("try again\n"));
				sleep(2);
			}
		}
        else
        {
            break;
        }
	}

	write(STDOUT_FILENO, buf, ret);
	close(fd);

	return 0;
}

