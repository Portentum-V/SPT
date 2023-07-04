
#include "Node_Send.h"

/*  send_message
* Send data via a socket, add session information followed by a message
*
* @int socket_descriptor: The socket to be used
* @char* buffer: The data to be sent
* @size_t buffer_size: The size of the data to be sent
*
* @Returns: -1 on failure, otherwise the number of bytes sent
*/
int send_message(conn_info* srv_info, int socket_descriptor, char* buffer, size_t buffer_size)
{
    int ret_val = 0;
    int msg_size = UUID_SIZE + buffer_size;
    char* msg = malloc(msg_size);
    if (msg == NULL) {
        fprintf(stderr, "send_message: Failed to malloc for msg");
        ret_val = -1;
        goto exit;
    }

    memcpy_s(msg, msg_size, srv_info->session_uuid, UUID_SIZE);
    memcpy_s(msg + UUID_SIZE - 2, msg_size, " ", 1);
    memcpy_s(msg + UUID_SIZE - 1, msg_size - UUID_SIZE, buffer, buffer_size);

    ret_val = send(socket_descriptor, msg, msg_size, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "send() failed with error %d\n", ret_val);
    }

    printf("Sent message: %s\n", msg);

exit:
    return ret_val;
}
