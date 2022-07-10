/***********************************************************/
/* Client_Win.h                                            */
/***********************************************************/

#pragma once
/* Unique Headers */
#include <winsock2.h>
#include <ws2tcpip.h>
/* IP_ADAPTER_INFO && PIP_ADAPTER_INFO */
#include <iphlpapi.h> 

#include "..\\Client_Utilities.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")


/* One day this will have a .h file */
int create_socket(char* srv_addr, char* srv_port, int int_sock, int conn_type);
int cleanup_socket(int socket_descriptor);

int get_mac_address(char* network_address);

int recv_message(int socket_descriptor, char* buffer, size_t buffer_size);
int echo_recv(int socket_descriptor);
int recv_file(conn_info* srv_info, char* file_name, int file_size);

int build_cmd_socket(conn_info* srv_info);

void print_wsaerror();