#pragma once

#include "Node_Networking.h"
#include "Node_Socket.h"

conn_info* init_conn_struct(char* str_addr, char* str_port, int sock_type);
void deinit_conn_struct(conn_info* srv_info);

int build_cmd_socket(conn_info* srv_info);
