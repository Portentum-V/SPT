#pragma once

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#endif

#if LINUX
	/* Nix */
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <netdb.h>
#endif

#include "..\\Utilities\\Node_Utilities.h"

/* Socket types */
#define SOCK_BAD	-1
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			        */

/* Connection types */
#define SOCK_CONN 1
#define SOCK_BIND 2

int create_socket(char* srv_addr, char* srv_port, int int_sock, int conn_type);

int cleanup_socket(int socket_descriptor);

int get_connection_information(int socket_descriptor);
