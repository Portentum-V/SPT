/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAXADDRSIZE 40
#define BUFFER 256

int create_socket(char* addr, char* port, int sock_type, int conn_type);

int get_connection_info(int sockfd);

int send_size(int sockfd, char**buf, uint32_t buf_size);
int recv_size(int sockfd, uint32_t *num);

uint32_t recv_until(int sockfd, char** data_buffer, uint32_t data_size);

// SSL Functions
int init_CTX(int method_type);
int load_certs(SSL_CTX* ctx, char* certfile, char* keyfile);