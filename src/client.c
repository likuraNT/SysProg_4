#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>



void argv_ip(int port, int argc, char* argv[])
{

}
void argv_switch();

int main(int argc, char* argv[])
{
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    int port = 7777;
    char chk_arg = '0';
    struct sockaddr_in sAddr;

    if (argc == 1)
    {
        sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        fprintf(stdout, "Using local address: 127.0.0.1\n");
    }

    else if (argc == 2)
    {
        int ind = -1;
        int len = strlen(argv[1]);
        for (int i = 0; i < len; ++i)
        {
            if (argv[1][i] == ':') {ind = i+1; break;}
        }

        char* cur_ip = malloc(ind);

        for (int i = 0; i < ind - 1; ++i)
        {
            cur_ip[i] = argv[1][i];
        }

        char local_buff[len - ind + 1];

        for (int i = 0; i < len-ind;++i )
        {
            local_buff[i] = argv[1][ind + i];
        }
        port = atoi (local_buff);

        sAddr.sin_addr.s_addr = inet_addr(cur_ip);
        fprintf(stdout, "Using different address: %s\n", argv[1]);
        free(cur_ip);
    }

    else if (argc == 3)
    {
        if (strcmp(argv[2], "manual") == 0) chk_arg = 'm';
        else if (strcmp(argv[2], "auto") == 0) chk_arg = 'a';
        else chk_arg = '0';  
    } 

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(port);

    connect(client_sock, (struct sockaddr*)&sAddr, sizeof(sAddr));

    int diff_num = -1;
    int* ptr_dn = &diff_num;
    void* buffer = malloc(sizeof(int));
    while(true)
    {
        if ((chk_arg == 'm' || chk_arg == '0') && *(int*)buffer != diff_num)
        {
            printf("Enter your num:\n");
            scanf("%i", &diff_num);
            send(client_sock, ptr_dn, sizeof(int), 0);

            ssize_t bytes_received = recv(client_sock, buffer, sizeof(int), 0);
            if (bytes_received == 0) {
                printf("Server has closed the connection.\n");
                break;
            }
        }
        else if (chk_arg == 'a' && *(int*)buffer != diff_num)
        {
            diff_num = rand()%101;
            fprintf(stdout, "My num is: %i\n", diff_num);
            send(client_sock, ptr_dn, sizeof(int), 0);

            ssize_t bytes_received = recv(client_sock, buffer, sizeof(int), 0);
                if (bytes_received == 0) {
                    printf("Server has closed the connection.\n");
                    break;
                }
        }
        else
        {
            fprintf(stdout, "Current num is: %i\n", *(int*)buffer);
            return 0;
        }
    }
    free(buffer);
    close(client_sock);
    return 0;
}
