EXECUTABLE_NAME = aesdsocket
CROSS_COMPILE = 0

CC ?= gcc
C_STANDARD = gnu99
CFLAGS = -g -Wall -Wextra
LDFLAGS = -lpthread -pthread

COMPILER_CALL = $(CC) $(CFLAGS) $(LDFLAGS)

all: aesdsocket
default: aesdsocket

aesdsocket: aesdsocket.o
	@$(COMPILER_CALL) aesdsocket.o -o $(EXECUTABLE_NAME)

clean:
	@rm -f *.o && rm -f $(EXECUTABLE_NAME)

aesdsocket.o:
	@$(COMPILER_CALL) aesdsocket.c -c
