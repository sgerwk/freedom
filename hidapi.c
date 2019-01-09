#include <stdlib.h>
#include <stdio.h>
#include <hidapi/hidapi.h>

#define WLEN 1024
#define OUTLEN 40
#define INLEN 20

void printkeys(unsigned char buf[INLEN], int res) {
	int i;

	for (i = 0; i < res; i++) {
		printf("%02X", buf[i]);
		if (i % 4 == 0)
			printf(" ");
	}
	printf("\n");
}

int main() {
	hid_device *handle;
	unsigned char onetwo[OUTLEN] = {0x31, 0x00, 0x00, 0x00};
	unsigned char threefour[OUTLEN] = {0x36, 0x06, 0x00, 0x00};
	unsigned char receive[INLEN];
	int res;

	hid_init();

	// handle = hid_open(0xe2b7, 0x0812, NULL);
	handle = hid_open(0x16d0, 0x05f0, NULL);
	if (handle == NULL) {
		printf("open device: %ls\n", hid_error(NULL));
		hid_exit();
		exit(EXIT_FAILURE);
	}

	res = hid_write(handle, onetwo, OUTLEN);
	printf("write: res=%d error=%ls\n", res, hid_error(handle));

	res = hid_read(handle, receive, INLEN);
	printf("read: res=%d error=%ls\n", res, hid_error(handle));
	printkeys(receive, res);

	res = hid_write(handle, threefour, OUTLEN);
	printf("write: res=%d error=%ls\n", res, hid_error(handle));

	res = hid_read(handle, receive, INLEN);
	printf("read: res=%d error=%ls\n", res, hid_error(handle));
	printkeys(receive, res);

	hid_close(handle);
	hid_exit();

	return EXIT_SUCCESS;
}

