# 定义编译器和编译选项
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -std=c11

# 目标文件
TARGET = writer
SRC = finder-app/writer.c
OBJ = $(SRC:.c=.o)

# 默认目标：构建应用程序
all: $(TARGET)

# 构建目标应用程序
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# 生成目标文件的规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标：删除生成的应用程序和所有.o文件
clean:
	rm -f $(TARGET) $(OBJ)

# 声明伪目标
.PHONY: all clean

