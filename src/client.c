#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

    int client_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(7777);

    if (argc == 1)
    {
        sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        fprintf(stdout, "Using local address: 127.0.0.1\n");
    }
    else
    {
        sAddr.sin_addr.s_addr = inet_addr(argv[1]);
        fprintf(stdout, "Using different address: %s\n", argv[1]);
    }

    connect(client_sock, (struct sockaddr*)&sAddr, sizeof(sAddr));

    int diff_num = -1;
    int* ptr_dn = &diff_num;
    while(true)
    {
        printf("Enter your num:\n");
        scanf("%i", &diff_num);
        send(client_sock, ptr_dn, sizeof(int), 0);

        void* buffer = malloc(sizeof(int));
        ssize_t bytes_received = recv(client_sock, buffer, sizeof(int), MSG_DONTWAIT);

        printf("%d", *(int*)buffer);

        if (&*(int*)buffer == diff_num)
        {
            fprintf(stdout, "Current num is %i", *(int*)buffer);
            break;
        }

        if (bytes_received == 0) {
            printf("Server has closed the connection.\n");
            break;
        }
    }
    close(client_sock);
    return 0;
}
