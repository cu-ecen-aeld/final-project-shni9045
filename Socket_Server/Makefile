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

all: server tempsensor
default: server tempsensor

server : server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o server server.o $(LDFLAGS)

server.o : server.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c server.c $(LDFLAGS)

tempsensor : sensor.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o tempsensor sensor.o $(LDFLAGS)

sensor.o : sensor.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c sensor.c $(LDFLAGS)

clean :
	rm -f *.o server tempsensor
