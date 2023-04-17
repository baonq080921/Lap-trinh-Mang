#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char message[BUFF_SIZE], server_reply[BUFF_SIZE];
    int port_number;

    // Kiểm tra đầu vào
    if (argc != 3) {
        printf("Sử dụng: %s <địa chỉ IP> <cổng>\n", argv[0]);
        return 1;
    }

    // Tạo socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Không thể tạo socket.\n");
        return 1;
    }

    // Thiết lập thông tin của server
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_family = AF_INET;
    port_number = atoi(argv[2]);
    server_address.sin_port = htons(port_number);

    // Kết nối đến server
    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("Không thể kết nối đến server.\n");
        return 1;
    }

    // Nhập thông tin của sinh viên
    printf("Nhập thông tin của sinh viên:\n");
    printf("MSSV: ");
    fgets(message, BUFF_SIZE, stdin);
    message[strcspn(message, "\n")] = '\0';
    strcat(message, ";");
    printf("Họ tên: ");
    fgets(server_reply, BUFF_SIZE, stdin);
    server_reply[strcspn(server_reply, "\n")] = '\0';
    strcat(message, server_reply);
    strcat(message, ";");
    printf("Ngày sinh: ");
    fgets(server_reply, BUFF_SIZE, stdin);
    server_reply[strcspn(server_reply, "\n")] = '\0';
    strcat(message, server_reply);
    strcat(message, ";");
    printf("Điểm trung bình các môn học: ");
    fgets(server_reply, BUFF_SIZE, stdin);
    server_reply[strcspn(server_reply, "\n")] = '\0';
    strcat(message, server_reply);

    // Gửi thông tin của sinh viên đến server
    if (send(client_socket, message, strlen(message), 0) < 0) {
        printf("Gửi thông tin của sinh viên thất bại.\n");
        return 1;
    }

    // Nhận phản hồi từ server
    if (recv(client_socket, server_reply, BUFF_SIZE, 0) < 0) {
        printf("Không thể nhận phản hồi từ server.\n");
        return 1;
    }

    printf("Phản hồi từ server: %s\n", server_reply);

    close(client_socket);
    return 0;
}
