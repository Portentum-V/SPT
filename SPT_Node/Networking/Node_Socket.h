#pragma once

#include "Node_Networking.h"

int create_socket(char* srv_addr, char* srv_port, int int_sock, int conn_type);

int cleanup_socket(int socket_descriptor);

int get_connection_information(int socket_descriptor);
