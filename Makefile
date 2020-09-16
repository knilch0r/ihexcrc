CC=gcc
CFLAGS=-O2 -Wall -W

OBJS=ihexcrc.o crc32.o

ihexcrc: $(OBJS) crc32.h

clean:
	rm -f $(OBJS)
