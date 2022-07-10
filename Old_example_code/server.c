#include "utilities.h"
#include <pthread.h>

#define BACKLOG 10

int server_accept_loop(int sockfd)
{
    int newfd = -1;
    struct sockaddr_storage remote_addr;
    socklen_t addr_size = sizeof(remote_addr);

    newfd = accept(sockfd, (struct sockaddr *)&remote_addr, &addr_size);
    if ( -1 == neewfd ) {
        fprintf(stderr, "Accept failed\n");
    }
    return newfd;
}

static void *server_thread_recv(void *void_sock)
{
    int retval = -1;
    uint32_t data_size = 0;
    char *data_buffer = {0};

    int sockfd = *((int*)void_sock);

    pthread_detach(pthread_self());
    printf("Thread created for socket %d\n", sockfd);

    retval = recv_size(sockfd, &data_size);
    if ( -1 == ret_val ) {
        fprintf(stderr, "Failed to recv size\n");
        goto exit;
    }

    data_buffer = malloc(data_size);
    if ( NULL == data_buffer) {
        fprintf(stderr, "Failed to malloc buffer for recv\n");
        goto exit;
    }

    retval = recv_until(socket, &data_buffer, data_size);
    if ( -1 == retval ) {
        fprintf(stderr, "Error during recv until\n");
        goto exit;
    }

    retval = 0;
    printf("Recvd: %s\n", data_buffer);

exit:
    close(sockfd);
    pthread_exit(retval);
}

int main(int argc, char* argv[])
{
    int retval = -1;
    pthread_t tid;
    char* str_port;
    int sock_type = -1;
    int socket = -1;
    int new_socket = -1;

    uint32_t data_size = 0;
    uint32_t tmp_Size = 0;
    char* data_buffer;

    char* buffer = malloc(BUFFER);
    if (buffer == NULL) {
        fprintf(stderr, "Buffer malloc failed\n");
        return -1;
    }
    memset(buffer, 0, BUFFER);

    if (argc<2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 0;
    }

    str_port = argv[1];
    sock_type = SOCK_STREAM;

    if (6 < strlen(str_port)) {
        fprintf(stderr, "Invalid port: %s\n", str_port);
        return -1;
    }

    printf("Attempt to bind to %s...\n", str_port);
    socket = create_socket(str_port, sock_type, 1, 2);
    if ( -1 == socket) {
        fprintf(stderr, "Socket creation failed\n");
        return -1;
    }

    /* Set the listen back log */
    retval = listen(socket, BACKLOG);
    if (-1 == retval) {
        fprintf(stderr, "Failed to listen\n");
        return -1;
    }

    /* Loop for connections */
    do {
        printf("Listening on port %s\n", str_port);
        new_socket = server_accept_loop(socket);
        if ( -1 = new_socket) {
            fprintf(stderr, "Accept failed\n");
            close(new_socket);
            continue;
        }
        printf("Connection! NEw sockfd created\n");
        get_connection_info(new_socket);
        
        /* Create a thread to handle the client */
        printf("Recinvg Size\n");
        retval = recv_size(new_socket, &data_size);
        if ( -1 == ret_val ) {
            fprintf(stderr, "Failed to recv size\n");
            close(new_socket);
            continue;
        }
        else if ( 0 == ret_val) {
            fprintf(stderr, "Recvd size of 0\n");
            close(new_socket);
            continue;
        }
        printf("Data size: %ld\n", data_size);

        data_buffer = malloc(data_size);
        if (NULL == data_buffer) {
            fprintf(stderr, "Failed to malloc buffer for recv\n");
            close(new_socket);
            continue;
        }
        memset(data_buffer, 0, data_size);

        printf("Expecting message of %ld bytes\n", data_size);
        tmp_size = recv_until(new_socket, &data_Buffer, data_size);
        if ( -1 == tmp_size) {
            fprintf(stderr, "Error during recv(): %d\n", errno);
            close(new_socket);
            continue;
        }

        printf("Buffer: %s\n", data_buffer);
        tmp_size = htonl(tmp_size);
        printf("Responding: %x\n", tmp_size);
        retval = send(new_socket, (void*)&tmp_size, 8, 0);

        close(new_socket);
        break;
    } while (1);

    close(socket);
    return 1;
}