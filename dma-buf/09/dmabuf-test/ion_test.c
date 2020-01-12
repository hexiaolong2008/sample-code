/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "ion.h"

#define PAGE_SIZE 4096

int main(int argc, char *argv[])
{
	int fd;
	struct ion_allocation_data alloc_data;

	fd = open("/dev/ion", O_RDWR);
	if (fd < 0) {
		printf("open /dev/ion failed, %s\n", strerror(errno));
		return -1;
	}

	alloc_data.len = 3 * PAGE_SIZE;
	if (ioctl(fd, ION_IOC_ALLOC, &alloc_data)) {
		printf("ion ioctl failed, %s\n", strerror(errno));
		close(fd);
		return -1;
	}

	printf("ion alloc success: size = %llu, dmabuf_fd = %u\n",
		alloc_data.len, alloc_data.fd);

	close(fd);

	return 0;
}
