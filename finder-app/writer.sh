#!/bin/bash
# Script to test finder-app and writer utility

# 清除之前的构建工件
make clean

# 使用本地编译器编译 writer 应用程序
make


# 变量定义
WRITER=./writer
WRITER_SCRIPT_PATH=$(realpath $(dirname $0))/writer.sh
TEMP_FILE=$(mktemp)

# 检查 writer 应用程序是否存在
if [ ! -f "$WRITER" ]; then
    echo "Error: writer application not found!"
    exit 1
fi

# 使用 writer 实用程序替代 writer.sh 脚本
echo "Using writer utility instead of writer.sh"

# 测试 writer 应用程序
$WRITER $TEMP_FILE "Test string for writer utility"
if [ $? -ne 0 ]; then
    echo "Error: writer utility failed to write to file!"
    exit 1
fi

# 读取文件内容并验证
CONTENT=$(cat $TEMP_FILE)
if [ "$CONTENT" != "Test string for writer utility" ]; then
    echo "Error: Content of file does not match expected string!"
    exit 1
fi

echo "Success: writer utility wrote the correct string to the file."

rm $TEMP_FILE

