#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

#define PORT 9000
#define DATA_FILE "/var/tmp/aesdsocketdata"

static volatile int keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 解析命令行参数，检查是否有 -d 参数
    int daemon_mode = 0;
    int c;

    while ((c = getopt(argc, argv, "d")) != -1) {
        switch (c) {
            case 'd':
                daemon_mode = 1;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (daemon_mode) {
        pid_t pid = fork();
        
        if (pid < 0) {
            exit(EXIT_FAILURE);
        }

        if (pid > 0) {
            // 父进程退出
            exit(EXIT_SUCCESS);
        }

        // 在子进程中
        if (setsid() < 0) {
            exit(EXIT_FAILURE);
        }

        // 关闭标准输入输出错误
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while (keep_running) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // 这里应该添加接收和处理数据的代码
        // Read data sent by the client
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Read failed");
        } else {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            printf("Received data: %s\n", buffer); // Just for example
        }
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

