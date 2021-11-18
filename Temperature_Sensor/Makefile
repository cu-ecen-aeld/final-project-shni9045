CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: tempsensor
default: tempsensor

tempsensor : sensor.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o tempsensor sensor.o $(LDFLAGS)

sensor.o : sensor.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c sensor.c $(LDFLAGS)

clean :
	rm -f *.o tempsensor
