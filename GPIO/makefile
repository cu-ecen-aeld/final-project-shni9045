
# the compiler: gcc for C program, define as g++ for C++
ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
ifeq ($(CFLAGS),)
	CFLAGS  = -g -Wall -Werror
endif

ifeq ($(LDFLAGS),)
	LDFLAGS = -pthread -lrt
endif

all: gpio
 
default: all

gpio: gpio.c
	$(CC) $(CFLAGS) -o gpio gpio.c $(LDFLAGS)

.PHONY: all

clean:
	rm -f gpio *.0
