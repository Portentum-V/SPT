#include "utilities.h"

/
int create_socket(char* addr, char* port, int sock_type, int conn_type)
{
    /* create_socket()
     * Generic wrapper for creating sockets
     * Args:
     *  char * addr: address (for connect) or NULL (for bind)
     *  char * port: remote (for connect) or local (for bind)
     *  int sock_type: tcp (1), udp (2), raw (3)
     *  int conn_type: connect (1), bind (2)
     * Returns:
     *  Socket descriptor or -1 (on error)
     */
    int socketfd = -1;
    int retval = -1;
    int sockopt = 1;

    struct addrinfo hints, *addr_info, *AI;
    memset(&hints, 0, sizeof(hints));

    /* Connect */
    if (1 == conn_type ) {
        hints.ai_flags      = AI_NUMERICSERV; /* no resolve servername */
        hints.ai_flags     |= AI_NUMERICHOST; /* no resolve hostname */
        hints.ai_family     = AF_UNSPEC;      /* IPv4 or IPv6 */
        hints.ai_socktype   = sock_type;      /* tcp 1, udp 2, raw 3 */
    }
    /* Bind */
    else {
        hints.ai_flags      = AI_PASSIVE; /* Need bind address */
        hints.ai_family     = AF_UNSPEC;  /* IPv4 or IPv6 */
        hints.ai_socktype   = sock_type;  /* tcp 1, udp 2, raw 3 */
    }

    retval = getaddrinfo(addr, port, &hints, &addr_info);
    if (retval != 0) {
        fprintf(stderr, "getaddrinfo() failed %s:%s, error: %d\n",
                addr, port, retval);
                goto FAIL;
    }

    /* Main Loop */
    for ( AI = addr_info; AI != NULL; AI = AI->ai_next) {
        /* Open Socket */
        socketfd = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);
        if (0 > sockfd) {
            fprintf(stderr, "Building socketfd with socket() failed, error %d\n", socketfd);
            continue;
        }

        if (2 == conn_type) {
            retval = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
            if (-1 == retval) {
                fprintf(stderr, "Setting SO_REUSEADDR failed, error: %d", retval);
                continue;
            }
        }

        printf("Successful socket call > family: %d | socktype: %d | protocol: %d\n",
               AI->ai_family, AI->ai_socktype, AI->ai_protocol);
        
        /* Connect */
        if ( 1 == conn_type && 2 != sock_type) {
            retval = connect(socketfd, AI->ai_addr, AI->ai_addrlen);
            if (0 > retval) {
                fprintf(stderr, "Connect() failed, error %d\n", retval);
                close(socketfd);
            }
            else break; /* Found a valid socket, break out of loop */
        }
        /* Bind */
        else if (2 == conn_type) {
            retval = bind(socketfd, AI->ai_Addr, AI->ai_addrlen);
            if (0 > retval) {
                fprintf(stderr, "Bind() failed, error %d\n", retval);
                close(socketfd);
            }
            else break; /* Found a valid socket, break out of loop */
        }
    }

    if (AI == NULL) {
        fprintf(stderr, "Unable to establish socket.\n");
        retval = -1;
        goto FAIL;
    }

    printf("Valid socket descriptor created: %d\n", socketfd);
    retval = socketfd;

FAIL:
    freeaddrinfo(addr_info);
    return retval;

}

int get_connection_info(int socketfd);
{
    int retval = -1;
    struct sockaddr_storage Addr;
    socklen_t addr_len = sizeof(Addr);
    char remote_addr_str[MAXADDRSIZE] = { 0 };
    char remote_port_str[32] = { 0 };
    char local_addr_str[MAXADDRSIZE] = { 0 };
    char local_port_str[32] = { 0 };

    retval = getpeername(socketfd, (struct sockaddr*)&Addr, &addr_len);
    if ( 0 > retval ) {
        fprintf(stderr, "getpeername() failed, error: %d\n", retval);
        return -1;
    }

    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len,
                    remote_addr_str, MAXADDRSIZE,
                    remote_port_str, 32,
                    NI_NUMERICHOST | NI_NUMERICSERV);
    }

    retval = getsockname(socketfd, (struct sockaddr*)&Addr, &addr_len);
    if (0 > retval) {
        fprintf(stderr, "getsockname() failed, error: %d\n", retval);
        return -1;
    }
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len,
                    local_addr_str, MAXADDRSIZE,
                    local_port_str, 32,
                    NI_NUMERICHOST | NI_NUMERICSERV);
    }
    printf("local| %s:%s <---> %s:%s |remote\n",
            local_addr_str, local_port_str,
            remote_addr_str, remote_port_str);
    return 0;
}

int send_size(int sockfd, char**buf, uint32_t buf_size);
{
    int retval = -1;
    uint32_t tmp;

    tmp = htonl(buf_size);
    printf("send_size()| SEnding size: %ld -> %ld\n", buf_size, tmp);
    retval = send(sockfd, (void *)&tmp, sizeof(uint32_t), 0);
    if (-1 == retval || 0 == retval) {
        fprintf(stderr, "send_size()| Failed to send size, error %d\n", errno);
        return retval;
    }
    printf("send_size()| Sending message: %s\n", *buf);
    retval = send(sockfd, *buf, buf_size, 0);
    if (-1 == retval || 0 == retval) {
        fprintf(stderr, "send_size()| Failed to send msg, error %d\n", errno);
        return retval;
    }
    printf("send_size()| Sent %d bytes. Waiting for response...\n", retval);
    return retval;
}

int recv_size(int sockfd, uint32_t *num);
{
    uint32_t size = 0;
    char *raw_size = (char*)&size;
    int retval = -1;
    int tmp_recv = 0;

    do {
        retval = recv(sockfd, raw_size, sizeof(uint32_t), 0);
        if (-1 == retval) {
            fprintf(stderr, "recv_size()| Retreiving data size failed\n");
            return -1;
        }
        tmp_recv += retval;
    } while( tmp_recv < sizeof(uint32_t));
    *num = ntohl(size);
    printf("recv_size()| Size: %x -> %x\n", size, *num);
    return tmprecv;
}

uint32_t recv_until(int sockfd, char** data_buffer, uint32_t data_size);
{
    int retval = 0;
    uint32_t tmp_recv = 0;

    do {
        retval = recv(sockfd, (*data_buffer)+tmp_recv, data_size, 0);
        if (-1 == retval) {
            fprintf(stderr, "recv_until()| Recv failed, error: %d\n", errno);
            return retval;
        }
        else if (0 == retval) {
            fprintf(stderr, "recv_until()| Clinet closed connection\n");
            break;
        }
        else {
            tmp_recv += ret_val;
        }
    } while( tmp_recv < data_size);

    return tmp_recv;
}

int init_CTX(int method_type)
{
    SSL_Method *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    if (1 == method_type) {
        method = TLSv1_2_client_method();
    }
    else if (2 == method_type) {
        method = TLSv1_2_server_method();
    }

    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        fprintf(stderr, "SSL NULL CTX\n");
        abort();
    }

    return ctx;
}

int load_certs(SSL_CTX* ctx, char* certfile, char* keyfile)
{
    int retval = -1;
    retval = SSL_CTX_use_certificate_file(ctx, certfile, SSL_FILETYPE_PEM);
    if (retval < 0) {
        fprintf(stderr, "load cert failed");
        return retval;
    }

    retval = SSL_CTX_use_PrivateKey_file(ctx, certfile, SSL_FILETYPE_PEM);
    if (retval < 0) {
        fprintf(stderr, "load key failed");
        return retval;
    }
    
    return retval;
}