#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;

	fd = open("/dev/my_dev", O_RDWR);
	if (fd < 0) {
		printf("open /dev/my_dev failed\n");
		return 0;
	}

	char *str = mmap(NULL, 4096 * 3, PROT_WRITE, MAP_SHARED, fd, 0);
	strcpy(str, "this is a test\n");
	munmap(str, 4096 * 3);

	str = mmap(NULL, 4096 * 3, PROT_READ, MAP_SHARED, fd, 0);
	printf("%s\n", str);
	munmap(str, 4096 * 3);

	close(fd);

	return 0;
}
