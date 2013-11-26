#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <errno.h>

#define CHAR_DEVICE "/dev/char_device"

int main(void){
	int fd;
	char buf[100]="abc";

	fd = open(CHAR_DEVICE, O_RDWR);
	if (fd < 0) {
		perror("open: ");
		return 1;
	}
	if (lseek(fd, 1, SEEK_SET) < 0) {
		perror("lseek: ");
		return 1;
	}
	if (write(fd, &buf, 4) < 0) {
		perror("write: ");
		return 1;
	}
	close(fd);
	return 0;
}