#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main()
{
	int ret;
	if ((ret = mkfifo("myfifo", 0600)) == -1)
	{
		perror("mkfifo error");
		exit(1);
	}

	return 0;
}

