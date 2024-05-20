#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 9000
#define DATA_FILE "/var/tmp/aesdsocketdata"

volatile sig_atomic_t running = 1;

void signal_handler(int signo)
{
    if (signo == SIGINT || signo == SIGTERM)
    {
        running = 0;
        syslog(LOG_INFO, "Caught signal, exiting");
    }
}

void cleanup(int server_socket)
{
    // Clean up and exit
    syslog(LOG_INFO, "Cleaning up and exiting");

    // Close server socket
    if (shutdown(server_socket, SHUT_RDWR) == -1)
    {
        perror("Error shutting down server socket");
    }

    if (close(server_socket) == -1)
    {
        perror("Error closing server socket");
    }

    // Delete the data file
    unlink(DATA_FILE);

    // Close syslog
    closelog();

    exit(EXIT_SUCCESS);
}

void daemonize()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        // Parent process exits
        exit(EXIT_SUCCESS);
    }

    // Child process continues
    umask(0);
    if (setsid() < 0)
    {
        perror("Error setting session ID");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void handle_client(int client_socket, int server_socket)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if (getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_len) == 0)
    {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        syslog(LOG_INFO, "Accepted connection from %s", client_ip);
    }

    char buffer[1024];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        int data_fd = open(DATA_FILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (data_fd == -1)
        {
            perror("Error opening data file");
            cleanup(server_socket);
        }

        write(data_fd, buffer, bytes_received);
        close(data_fd);

        if (memchr(buffer, '\n', bytes_received) != NULL)
        {
            int file_fd = open(DATA_FILE, O_RDONLY);
            if (file_fd == -1)
            {
                perror("Error opening data file for reading");
                cleanup(server_socket);
            }

            char file_buffer[1024];
            ssize_t bytes_read;

            while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0)
            {
                send(client_socket, file_buffer, bytes_read, 0);
            }

            close(file_fd);
            break;
        }
    }

    syslog(LOG_INFO, "Closed connection from %s", inet_ntoa(client_addr.sin_addr));
    close(client_socket);
}

int main(int argc, char *argv[])
{
    int daemon_mode = 0;

    if (argc > 1 && strcmp(argv[1], "-d") == 0)
    {
        daemon_mode = 1;
    }

    openlog("aesdsocket", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Starting aesdsocket");

    // Set up signal handlers using sigaction for better control
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("Error setting up signal handlers");
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        cleanup(server_socket);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        cleanup(server_socket);
    }

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding socket");
        cleanup(server_socket);
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("Error listening for connections");
        cleanup(server_socket);
    }

    if (daemon_mode)
    {
        daemonize();
    }
    while (running)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1)
        {
            perror("Error accepting connection");
            cleanup(server_socket);
        }

        handle_client(client_socket, server_socket);
    }

    cleanup(server_socket); // Cleanup if the loop exits

    return 0;
}

