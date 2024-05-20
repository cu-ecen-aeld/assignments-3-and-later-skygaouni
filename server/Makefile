CC = gcc
CFLAGS = -Wall -Wextra 
LDFLAGS = -lpthread

all: aesdsocket

aesdsocket: aesdsocket.c
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket $(LDFLAGS)

clean:
	rm -f aesdsocket
