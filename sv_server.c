#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int server_socket;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char log_file[BUF_SIZE];
    time_t current_time;
    char *client_address;
    char time_string[BUF_SIZE];
    FILE *log_ptr;

    if (argc != 3) {
        printf("Usage: %s <port> <log file>\n", argv[0]);
        return -1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        return -1;
    }

    // Prepare server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to bind socket");
        return -1;
    }

    // Start listening for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("Failed to listen for connections");
        return -1;
    }

    // Open the log file for writing
    snprintf(log_file, BUF_SIZE, "%s.log", argv[2]);
    log_ptr = fopen(log_file, "a");
    if (log_ptr == NULL) {
        perror("Failed to open log file");
        return -1;
    }

    printf("Server is listening on port %s...\n", argv[1]);

    while (1) {
        int client_socket;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Wait for a client to connect
        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            return -1;
        }

        // Get the client's IP address
        client_address = inet_ntoa(client_addr.sin_addr);

        // Get the current time
        current_time = time(NULL);
        strftime(time_string, BUF_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&current_time));

        // Receive data from the client
        if (recv(client_socket, buffer, BUF_SIZE, 0) == -1) {
            perror("Failed to receive data from client");
            return -1;
        }

        // Print the data to the console and write it to the log file
        printf("%s %s %s\n", client_address, time_string, buffer);
        fprintf(log_ptr, "%s %s %s\n", client_address, time_string, buffer);

        // Close the client socket
        close(client_socket);
    }

    // Close the log file
    fclose(log_ptr);

    // Close the server socket
    close(server_socket);
    return 0;
