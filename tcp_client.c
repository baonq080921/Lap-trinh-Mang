#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Sử dụng: %s <địa chỉ IP> <cổng>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Lấy địa chỉ IP và cổng từ tham số dòng lệnh
    char *ip = argv[1];
    int port = atoi(argv[2]);

    // Tạo socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Không thể tạo socket");
        exit(EXIT_FAILURE);
    }

    // Kết nối đến server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Không thể kết nối đến server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Nhập dữ liệu từ bàn phím và gửi đến server
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Nhập dữ liệu để gửi đến server: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Không thể gửi dữ liệu đến server");
            break;
        }

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    // Đóng kết nối
    close(client_socket);
    return 0;
}
