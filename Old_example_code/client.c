#include "utilities.h"

int main(int argc, char* argv[])
{
    char* str_addr;
    char* str_port;
    int sock_type = -1;
    int conn_type = -1;
    int socket = -1;
    int retval = -1;

    uint32_t buf_size = 0;
    char *buf = { 0 };

    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    str_addr = argv[1];
    str_port = argv[2];
    sock_type = SOCK_STREAM;
    conn_type = 1; /* connect (1) | bind (2) */

    if ( 40 < strlen(str_addr)) {
        fprintf(stderr, "Invalid address: %s\n", str_addr);
        return -1;
    }

    if ( 6 < strlen(str_port)) {
        fprintf(stderr, "Invalid port: %s\n", str_port);
        return -1;
    }

    printf("Attempting to connect to %s:%s\n", str_addr, str_port);
    socket = create_socket(str_addr, str_port, sock_type, conn_type);
    if (-1 == socket) {
        return -1;
    }

    get_connection_info(socket);
    buf_size = strlen(buf);
    retval = send_size(socket, &buf, buf_size);

    printf("Snet %d bytes\nWaiting for response...\n", retval);
    retval = recv_size(socket, &buf_size);
    printf("Server recevied %x bytes\n", ntohl(buf_size));
    return 1;
}