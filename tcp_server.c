#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    int port = atoi(argv[1]);
    char buffer[BUFFER_SIZE];
    int file_descriptor;
    ssize_t bytes_received, bytes_written;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to a specific port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    memset(&(server_address.sin_zero), 0, 8);
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Open file for reading greeting message
    if ((file_descriptor = open(argv[2], O_RDONLY)) < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Read greeting message and send it to client
    while ((bytes_received = read(file_descriptor, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = send(client_fd, buffer, bytes_received, 0);
        if (bytes_written < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close greeting message file
    close(file_descriptor);

    // Open file for writing client data
    if ((file_descriptor = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Receive client data and write it to file
    while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        bytes_written = write(file_descriptor, buffer, bytes_received);
        if (bytes_written < 0) {
            perror("write failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close client data file and socket
    close(file_descriptor);
    close(client_fd);

    return 0;
}
