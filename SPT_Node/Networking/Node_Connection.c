
#include "Node_Connection.h"

/* Takes user input and returns a struct of addr, port, protocol if successful otherwise NULL */
// conn_info * menu(int argc, char* argv[])
conn_info* init_conn_struct(char* str_addr, char* str_port, int sock_type)
{
    conn_info* srv_info = NULL;

    // Valdiate input
    JMP_IF(40 < sizeof(str_addr), ERRORCODE_INPUT, FAIL, "Invalid address: %s", str_addr);
    JMP_IF(-1 == convert_port(str_port), ERRORCODE_INPUT, FAIL, "Invalid Port: %s", str_port);
    
    // Malloc and assign values to the struct
    srv_info = (struct conn_info*)malloc(sizeof(struct conn_info));
    JMP_IF(NULL == srv_info, ERRORCODE_ALLOCATE, FAIL, "menu: conn_info malloc failed - NULL ptr");

    strcpy_s(srv_info->addr, sizeof(srv_info->addr), str_addr);
    strcpy_s(srv_info->port, sizeof(srv_info->port), str_port);
    srv_info->socket_type = sock_type;
    strcpy_s(srv_info->session_uuid, sizeof(srv_info->session_uuid), UUID_NONE);

    // Set default stuffs
    srv_info->socket_descriptor = -1;

    log_debug("conn_info created -  %s:%s|%d\n", str_addr, str_port, sock_type);
    return srv_info;

FAIL:
    if (NULL != srv_info)
        free(srv_info);
    return NULL;

}

void deinit_conn_struct(conn_info* srv_info)
{
    if (NULL != srv_info) {
        if (-1 < srv_info->socket_descriptor) {
            cleanup_socket(srv_info->socket_descriptor);
        }
        free(srv_info);
        g_error = ERRORCODE_SUCCESS;
    }
    g_error = ERRORCODE_NULL;
}

/*  build_cmd_socket
* By the end of this function, should have a cmd_socket and session_uuid set in the conn_info struct
*
* @conn_info* srv_info: Relevent conn_info struct that holds server information.
*
* @Returns: -1 on failure, otherwise 0
*/
int build_cmd_socket(conn_info* srv_info)
{

    int ret_val = 0;

    char* buffer = malloc(BUFFER);
    JMP_IF(buffer == NULL, ERRORCODE_ALLOCATE, FAIL, "Build_cmd_socket: buffer Malloc failed - NULL ptr\n");
    memset(buffer, 0, BUFFER);

    srv_info->socket_descriptor = create_socket(srv_info->addr, srv_info->port, srv_info->socket_type, SOCK_CONN);
    JMP_IF(srv_info->socket_descriptor == -1, ERRORCODE_SOCKET, FAIL, "Build_cmd_socket: create_socket failed\n");
    //printf("cmd_socket successfully set: %d\n", srv_info->cmd_socket);

    /* Send empty session_uuid (0000) and recv actual session_uuid */
    // Here is 'message' should be encoded, compressed, encrypted before being sent
    ret_val = send_message(srv_info, srv_info->socket_descriptor, "init", 5); //Playing with struct like [ UUID CMMD SIZE ]
    //ret_val = send(srv_info->cmd_socket, srv_info->session_uuid, UUID_SIZE, 0);
    if (ret_val == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to send message\n");
        goto FAIL;
    }
    ret_val = recv(srv_info->socket_descriptor, buffer, UUID_SIZE, 0);
    if (ret_val == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to recv message\n");
        goto FAIL;
    }
    /*
    else {
        printf("Message: %s", buffer);
    } */
    strncpy_s(srv_info->session_uuid, UUID_SIZE, buffer, UUID_SIZE);
    printf("Session UUID succesfully set: %s\n", srv_info->session_uuid);

FAIL:
    free(buffer);
    return ret_val;
}


/*  build_data_socket
*
*
*
*/
int build_data_socket(conn_info* srv_info)
{
    int ret_val = 0;

    char* buffer = malloc(BUFFER);
    if (buffer == NULL) {
        fprintf(stderr, "Build_cmd_socket: buffer Malloc failed - NULL\n");
        ret_val = -1;
        goto exit;
    }
    memset(buffer, 0, BUFFER);

    //Check that a session UUID is set
    if (!strncmp(srv_info->session_uuid, "000000000-0000-0000-0000-000000000000", UUID_SIZE)) {
        fprintf(stderr, "Build_cmd_socket: Session UUID not set\n");
        ret_val = -1;
        goto exit;
    }

    // Eventually specify the addr:port/protocol to receive from?
    /*
    srv_info->data_socket = create_socket(srv_info->addr, srv_info->port, srv_info->socket_type, 2);
    if (srv_info->data_socket == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to build data socket\n");
        ret_val = -1;
        goto exit;
    } 
    */

FAIL:
exit:
    free(buffer);
    return ret_val;
}