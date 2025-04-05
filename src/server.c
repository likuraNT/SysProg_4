#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

void log_req(const char* c_ip, int req) {
    fprintf(stdout, "<ip: %s>\t<request: %i>\n", c_ip, req);
}

int main(int argc, char* argv[]) {
    int port = 0;

    if (argc < 2) {
        port = 7777;
        fprintf(stdout, "Using default port 7777\n");
    } else {
        port = atoi(argv[1]);
        fprintf(stdout, "Using different port: %i\n", port);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(port);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr*)&sAddr, sizeof(sAddr));
    listen(server_sock, 5);

    fprintf(stdout, "Waiting for connections...\n");

    while (true) {
        int client_sock = accept(server_sock, NULL, NULL);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        char c_ip[INET_ADDRSTRLEN];
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        getpeername(client_sock, (struct sockaddr*)&client_addr, &addr_len);
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, c_ip, sizeof(c_ip));

        fprintf(stdout, "Ip: %s has been connected.\n", c_ip);

        void* buff = malloc(sizeof(int));

        int curr_num = rand() % 101;
        int diff_num = -1;

        while (true) {
            ssize_t bytes_received = recv(client_sock, buff, sizeof(int), MSG_WAITALL);
            if (bytes_received <= 0) {
                fprintf(stdout, "Client disconnected or error occurred.\n");
                break;
            }

            diff_num = *(int*)buff;
            log_req(c_ip, diff_num);

            if (curr_num > diff_num) {
                fprintf(stdout, "My number is higher than you: %i\n", diff_num);
                *(int*)buff = -1;
            } else if (curr_num < diff_num) {
                fprintf(stdout, "My number is lower than you: %i\n", diff_num);
                *(int*)buff = -1;
            } else {
                fprintf(stdout, "Correct!\n");
                send(client_sock, (int*)buff, sizeof(int), 0);
                break;
            }
            send(client_sock, (int*)buff, sizeof(int), 0);
        }

        free(buff);
        close(client_sock);
        fprintf(stdout, "Ip: %s has been disconnected.\n", c_ip);
    }

    close(server_sock);
    return 0;
}
