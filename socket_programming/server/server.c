#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

#define MAX_BUFFER_SIZE 1024

// 문자열을 대문자로 변환하는 함수
void toUpperCase(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

// 문자열을 소문자로 변환하는 함수
void toLowerCase(char* str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

// 문자열의 첫 글자를 대문자로 변환하는 함수
void capitalizeFirstLetter(char* str) {
    if (*str) {
        *str = toupper((unsigned char)*str);
    }
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];
    int bytes_sent, bytes_received;

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "init fail..\n");
        return 1;
    }

    // 소켓 생성
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        fprintf(stderr, "Failed\n");
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 소켓에 주소 할당
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // 연결 대기
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "Failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("The server is ready to receive\n");

    // 클라이언트 연결 수락
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == INVALID_SOCKET) {
        fprintf(stderr, "Failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("client connected.\n");

    while (1) {
        // 클라이언트로부터 명령 수신
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';

            if (strcmp(buffer, "1") == 0) {
                // 대문자 변환
                // 클라이언트로부터 데이터 수신
                bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    // 대문자로 변환
                    toUpperCase(buffer);
                    // 클라이언트에게 결과 전송
                    bytes_sent = send(client_socket, buffer, bytes_received, 0);
                    if (bytes_sent == SOCKET_ERROR) {
                        fprintf(stderr, "Failed\n");
                        break;
                    }
                }
                else {
                    fprintf(stderr, "Failed\n");
                    break;
                }
            }
            else if (strcmp(buffer, "2") == 0) {
                // 소문자 변환
                // 클라이언트로부터 데이터 수신
                bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    // 소문자로 변환
                    toLowerCase(buffer);
                    // 클라이언트에게 결과 전송
                    bytes_sent = send(client_socket, buffer, bytes_received, 0);
                    if (bytes_sent == SOCKET_ERROR) {
                        fprintf(stderr, "Failed\n");
                        break;
                    }
                }
                else {
                    fprintf(stderr, "Failed\n");
                    break;
                }
            }
            else if (strcmp(buffer, "3") == 0) {
                // 첫 글자 대문자로 변환
                // 클라이언트로부터 데이터 수신
                bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    // 첫 글자 대문자로 변환
                    capitalizeFirstLetter(buffer);
                    // 클라이언트에게 결과 전송
                    bytes_sent = send(client_socket, buffer, bytes_received, 0);
                    if (bytes_sent == SOCKET_ERROR) {
                        fprintf(stderr, "Failed\n");
                        break;
                    }
                }
                else {
                    fprintf(stderr, "Failed\n");
                    break;
                }
            }
            else if (strcmp(buffer, "4") == 0) {
                // 종료
                printf("close...\n");
                break;
            }
            else {
                fprintf(stderr, "Failed\n");
                break;
            }
        }
        else {
            fprintf(stderr, "Failed\n");
            break;
        }
    }

    // 연결 종료
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
