#pragma once

#include "..\\Utilities\\Node_Utilities.h"

/* Socket types */
#define SOCK_BAD	-1
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			        */

/* Connection types */
#define SOCK_CONN 1
#define SOCK_BIND 2

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