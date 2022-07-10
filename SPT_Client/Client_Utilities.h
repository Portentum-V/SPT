/***********************************************************/
/* Client_Utilities.h                                      */
/***********************************************************/

#pragma once
/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
#include <winsock2.h>
#include <ws2tcpip.h>              /* Provides getnameinfo for get_connection_information */
#pragma comment(lib, "Ws2_32.lib") /* Provides ws2tcpip.h */
#endif

#include "Client_Error.h"

/* Constants */
#define MAXADDRSIZE 40 
#define HWADDRSIZE 13
#define BUFFER 256
#define UUID_SIZE 39

#define VENDORSIZE 13
#define MODELSIZE 49

/* Socket types */
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			        */

/* Connection types */
#define SOCK_CONN 1
#define SOCK_BIND 2

/* Structs */
typedef struct conn_info {
    /* Network information */
    char srv_addr[MAXADDRSIZE];
    char srv_port[6]; // Max port: 65535 (5 chars + Null = 6)
    int int_sock;
    /* Sockets */
    int cmd_socket;
    int data_socket;
    int shell_socket;
    /* Session information */
    char session_uuid[UUID_SIZE];

} conn_info;

/* Utility functions */
int check_port(char* input);
conn_info* menu(int argc, char* argv[]);

int get_connection_information(int socket_descriptor);

int send_message(conn_info* srv_info, int socket_descriptor, char* buffer, size_t buffer_size);
