CC = gcc

CROSS_COMPILE = aarch64-none-linux-gnu-

TARGET = writer

SRC = writer.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $< -o $@

cross: CC = $(CROSS_COMPILE)gcc
cross: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all cross clean
