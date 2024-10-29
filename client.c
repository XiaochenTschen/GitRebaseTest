#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#include "socket_helper.h"

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int socketfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char buf[BUFFER_SIZE];
    char msg[BUFFER_SIZE];
    struct timeval timeout;

    /* do some changes here */
    if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* initialize the server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invaliad address");
        close(socketfd);
        return 1;
    }

    /* set the client receive timeout */
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;
    if(setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Set socket receive timeout failed");
        close(socketfd);
        return 1;
    }

    printf("Enter message to send to server ('exit' to quit): \n");

    while(1) {
        flush_socket_recv_buffer(socketfd);

        printf("> ");
        fgets(msg, BUFFER_SIZE, stdin);
        msg[strcspn(msg, "\n")] = 0;

        if(strcmp(msg, "exit") == 0) {
            break;
        }

        set_nonblock_mode(socketfd, 1);
        ssize_t bytes_sent = sendto(socketfd, msg, strlen(msg), 0, (const struct sockaddr*)&server_addr, sizeof(server_addr));
        
        if(bytes_sent < 0) {
            perror("Send Failed");
        }

        set_nonblock_mode(socketfd, 0);
        ssize_t byte_receiv = recvfrom(socketfd, buf, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len);
        if(byte_receiv < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("Client receive timeout..\n");
            }
            else
                perror("Receive failed");
        }
        else {
            buf[byte_receiv] = '\0';
            printf("%s\n", buf);
        }
        sleep(2);
    }

    close(socketfd);
    printf("Client closed\n");
    return 0;
}
