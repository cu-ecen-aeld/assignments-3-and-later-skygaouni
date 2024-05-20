CC ?= gcc
CFLAGS = -Wall -Werror
TARGET = aesdsocket

all: $(TARGET)
default: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)

