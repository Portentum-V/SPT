#pragma once

#include "..\\Utilities\\Node_Utilities.h"

#include "Node_Connection.h"

int send_message(conn_info* srv_info, int socket_descriptor, char* buffer, size_t buffer_size);;
