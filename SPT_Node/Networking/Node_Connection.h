#pragma once

#include "Node_Socket.h"

#include "..\\Utilities\\Node_Utilities.h"

/* Structs */
typedef struct conn_info {
    /* Network information */
    char addr[MAXADDRSIZE];
    char port[6]; // Max port: 65535 (5 chars + Null = 6)
    /* Socket */
    int socket_type;
    int socket_descriptor;
    /* Session information */
    char session_uuid[UUID_SIZE];

} conn_info;

conn_info* init_conn_struct(char* str_addr, char* str_port, int sock_type);
void deinit_conn_struct(conn_info* srv_info);

int build_cmd_socket(conn_info* srv_info);
