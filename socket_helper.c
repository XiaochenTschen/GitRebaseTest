/**
 * Implementation of helper functions
 */
#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include "socket_helper.h"

void set_nonblock_mode(int scokfd, int enable) {
    int flags = fcntl(scokfd, F_GETFL, 0);
    if(enable)
        fcntl(scokfd, F_SETFL, flags | O_NONBLOCK);
    else
        fcntl(scokfd, F_SETFL, flags & ~O_NONBLOCK);
}

void flush_socket_recv_buffer(int sockfd) {
    printf("enter flush function\n");
    int flags = fcntl(sockfd, F_GETFL, 0);
    set_nonblock_mode(sockfd, 1);

    char buffer[1024];
    while(1) {
        int byte_receive = recvfrom(sockfd, buffer, 1024, 0, NULL, NULL);
        if(byte_receive < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                /* socket buffer is empty*/
                break;
            }
            else {
                /* receive error */
                break;
            }
        }
        else if (byte_receive == 0) {
            /* should not happen for UDP */
            break;
        }
        else {
            buffer[byte_receive] = '\0';
            printf("Discarded message: %s\n", buffer);
        }
    }

    fcntl(sockfd, F_SETFL, flags);
    printf("exit flush function\n");
}