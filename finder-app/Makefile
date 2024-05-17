CC := gcc
CROSS_COMPILE :=

ifdef CROSS_COMPILE
	CC := $(CROSS_COMPILE)$(CC)
endif

.PHONY: clean

writer: writer.o
	$(CC) -o writer writer.o

writer.o: writer.c
	$(CC) -c writer.c

clean:
	rm -f writer *.o
