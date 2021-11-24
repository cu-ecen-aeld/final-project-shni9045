CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

ifeq ($(LDFLAGS),)
	LDFLAGS= -lrt
endif

all: server
default: server

server : server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o server server.o $(LDFLAGS)

server.o : server.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c server.c $(LDFLAGS)

clean :
	rm -f *.o server
