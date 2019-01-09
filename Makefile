PROGS=hidapi hex2char

CFLAGS=-g -Wall -Wextra
LDFLAGS=-lhidapi-hidraw

all: ${PROGS}

clean:
	rm -f ${PROGS}

