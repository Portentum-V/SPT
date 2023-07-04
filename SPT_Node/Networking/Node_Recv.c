
#include "Node_Recv.h"

/*  recv_message
* Receive data  via a socket
*
* @int socket_descriptor: The socket to be used
* @char* buffer: The buffer to save the data in
* @size_t buffer_size: The amount of data to receive
*
* @Returns: -1 on failure, otherwise the number of bytes.
*/
int recv_message(int socket_descriptor, char* buffer, size_t buffer_size)
{
    int recv_amount;

    recv_amount = recv(socket_descriptor, buffer, buffer_size, 0);
    if (-1 == recv_amount) {
        fprintf(stderr, "recv() failed with error %d\n", recv_amount);
    }
    else if (0 == recv_amount) {
        printf("Recevied 0\n");
    }
    else {
        printf("Message: %s\n", buffer);
    }

    return recv_amount;
}


/*  recv_file
*
*
*
*/
int recv_file(conn_info* srv_info, char* file_name, int file_size)
{
    int i;
    int ret_val = 0;
    int bytes_recvd = 0;
    FILE* new_file = NULL;

    void* recv_buffer = malloc(BUFFER);
    if (recv_buffer == NULL) {
        fprintf(stderr, "recv_file: failed to malloc buffer");
        ret_val = -1;
        goto EXIT;
    }

    ret_val = fopen_s(&new_file, file_name, "wb");
    if (new_file == NULL) {
        fprintf(stderr, "Failed to open file %s", file_name);
        ret_val = -1;
        goto EXIT;
    }
    /*
    srv_info->data_socket = create_socket(srv_info->addr, srv_info->port, srv_info->socket_type, 2);
    if (-1 == srv_info->data_socket) {
        printf("Failed to build socket for file download: %s", file_name);
        ret_val = -1;
        goto EXIT;
    }

    // Send UUID and expected file?
    ret_val = send(srv_info->data_socket, srv_info->session_uuid, UUID_SIZE, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "recv_file: UUID send() failed with error %d\n", ret_val);
        goto EXIT;
    }
    ret_val = send(srv_info->data_socket, file_name, strlen(file_name) + 1, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "recv_file: file_name send() failed with error %d\n", ret_val);
        goto EXIT;
    }

    printf("Socket established and ACK for %s sent\n", file_name);

    for (i = 0; i < file_size; i += bytes_recvd) {
        bytes_recvd = recv(srv_info->data_socket, recv_buffer, BUFFER, 0);
        if (-1 == bytes_recvd) {
            fprintf(stderr, "recv() failed with error %d\n", bytes_recvd);
            ret_val = -1;
            goto EXIT;
        }
        else if (0 == bytes_recvd) {
            printf("Reached end of file\n");
        }
        fwrite(recv_buffer, BUFFER, 1, new_file);
    }
    */
EXIT:
    if (new_file != NULL) {
        fclose(new_file);
    }
    /*
    if (srv_info->data_socket != -1) {
        cleanup_socket(srv_info->data_socket);
        srv_info->data_socket = -1;
    } */
    free(recv_buffer);
    return ret_val;

}