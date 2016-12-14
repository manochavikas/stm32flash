#include <linux/termios.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int ioctl(int d, int request, ...);
uint32_t update_serial(const char *device, unsigned int speed, int inc_or_set_flag)
{
	int fd;
	struct termios2 tio;

	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		fprintf(stderr, "error opening %s: %s", device, strerror(errno));
		return 2;
	}
	if (ioctl(fd, TCGETS2, &tio)) {
		perror("TCGETS2");
		return 3;
	}
	//printf("speed before change is = %d\n", tio.c_ospeed);
	tio.c_cflag &= ~CBAUD;
	tio.c_cflag |= BOTHER;
	if(inc_or_set_flag == 1) {
		tio.c_ispeed += speed;
		tio.c_ospeed += speed;
	}
	else if (inc_or_set_flag == 2) {
		tio.c_ispeed = speed;
		tio.c_ospeed = speed;
	}
	else {
		perror("Not valid flag passed to update_serial\n");
		return -1;
	}
	/* do other miscellaneous setup options with the flags here */
	if(ioctl(fd, TCSETS2, &tio)) {
		perror("TC_SETS2");
		return 4;	
	}
	if (ioctl(fd, TCGETS2, &tio)) {
		perror("TCGETS2 after setting");
		return 5;
	}
	//printf("actual speed reported %d\n", tio.c_ospeed);
	tcflush(fd, TCIFLUSH);
	close(fd);
	return tio.c_ospeed;
}

uint32_t get_current_baud_rate(const char *device)
{
	int fd;
	struct termios2 tio;

	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		fprintf(stderr, "error opening %s: %s", device, strerror(errno));
		return 2;
	}
	if (ioctl(fd, TCGETS2, &tio)) {
		perror("TCGETS2");
		return 3;
	}
	close(fd);
	return tio.c_ospeed;
}
