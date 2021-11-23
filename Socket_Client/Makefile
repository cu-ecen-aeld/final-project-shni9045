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

all: client
default: client

client : client.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o client client.o $(LDFLAGS)

client.o : client.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c client.c $(LDFLAGS)

clean :
	rm -f *.o client
