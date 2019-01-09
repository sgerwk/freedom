#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int main() {
	char buf[2];
	int c;

	// todo: skip newlines

	while (2 == fread(buf, 1, 2, stdin)) {
		sscanf(buf, "%02X", &c);
		if (isprint(c))
			printf("%c", c);
		else
			printf("_");
	}
	printf("\n");
}

