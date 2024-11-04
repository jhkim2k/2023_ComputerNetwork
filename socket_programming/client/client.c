#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

#define MAX_BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int bytes_sent, bytes_received;

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "From Server (Fail)\n");
        return 1;
    }

    // 소켓 생성
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        fprintf(stderr, "From Server (Fail)\n");
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);

    // 서버 주소를 127.0.0.1로 설정
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "From Server (Fail)\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // 서버에 연결
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "From Server (Fail)\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    while (1) {
        printf("1. UPPER message, 2. LOWER message, 3. CAPITALIZE message, 4. EXIT\n");
        printf("Enter command: ");
        fflush(stdout);

        // 사용자로부터 명령 입력
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // 서버에 명령 전송
        bytes_sent = send(client_socket, buffer, (int)strlen(buffer), 0);
        if (bytes_sent == SOCKET_ERROR) {
            fprintf(stderr, "From Server (Fail)\n");
            break;
        }

        if (strcmp(buffer, "4") == 0) {
            // 종료
            printf("close...\n");
            break;
        }

        // 명령에 따라 데이터 처리
        if (strcmp(buffer, "1") == 0 || strcmp(buffer, "2") == 0 || strcmp(buffer, "3") == 0) {
            printf("Enter data: ");
            fflush(stdout);

            // 사용자로부터 데이터 입력
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';

            // 서버에 데이터 전송
            bytes_sent = send(client_socket, buffer, (int)strlen(buffer), 0);
            if (bytes_sent == SOCKET_ERROR) {
                fprintf(stderr, "From Server (Fail)\n");
                break;
            }

            // 서버로부터 결과 수신
            bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                printf("From Server (Success): %s\n", buffer);
            }
            else {
                fprintf(stderr, "From Server (Fail)\n");
                break;
            }
        }
        else {
            printf("From Server (Fail)\n");
        }
    }

    // 정리
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
