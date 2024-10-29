/**
 * helper functions while use socket
 */

#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H

#include <arpa/inet.h>
#include <fcntl.h>

void set_nonblock_mode(int scokfd, int enable);

void flush_socket_recv_buffer(int socket);

#endif /* SOCKET_HELPER_H */