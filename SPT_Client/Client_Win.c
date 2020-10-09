/**********************************************************/
/* Client_Win.c                                           */
/* Provides IPv4 or IPv6 TCP connections for *Nix devices */
/**********************************************************/

/* Unique Headers */
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Client_Utilities.h"

// Link with ws2_32.lib <- Not sure if this is required? -> It is...
#pragma comment(lib, "Ws2_32.lib")

/* Once a bit further, these should be moved to Client_Utilities.h*/
int remote_connect(char* srv_addr, char* srv_port, int int_sock);
int cleanup_socket(int socket_descriptor);
int get_connection_information(int socket_descriptor, struct addrinfo* AI);
int send_message(int socket_descriptor, char* buffer, size_t buffer_size);
int recv_message(int socket_descriptor, char* buffer, size_t buffer_size);

int build_cmd_socket(conn_info* srv_info);

/* This will contain the main loop that responds to commands from the server */
int main(int argc, char* argv[])
{
    int exit_status = 0;
    conn_info* srv_info = malloc(sizeof(conn_info));
    if (srv_info == NULL) {
        fprintf(stderr, "Failed to malloc conn_info\n");
        exit_status = -1;
        goto exit;
    }
    memset(srv_info, 0, sizeof(conn_info));

    if ((srv_info = menu(argc, argv)) == NULL) {
        fprintf(stderr, "Failed to get input from menu\n");
        exit_status = -1;
        goto exit;
    }

    exit_status = build_cmd_socket(srv_info);
    if (exit_status == -1) {
        fprintf(stderr, "build_cmd_socket failed\n");
        goto exit;
    }

    /* Clean Exit */
exit:
    if (srv_info != NULL) {
        if (-1 < srv_info->cmd_socket) {
            cleanup_socket(srv_info->cmd_socket);
        }
        if (-1 < srv_info->data_socket) {
            cleanup_socket(srv_info->data_socket);
        }
        if (-1 < srv_info->shell_socket) {
            cleanup_socket(srv_info->data_socket);
        }
    }
    free(srv_info);
    printf("Clean Exit\n");
    exit(exit_status);
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
    char *buffer = malloc(BUFFER);
    if (buffer == NULL) {
        fprintf(stderr, "Build_cmd_socket: Malloc failed: NULL\n");
        ret_val = -1;
        goto exit;
    }
    memset(buffer, 0, BUFFER);

    srv_info->cmd_socket = remote_connect(srv_info->srv_addr, srv_info->srv_port, srv_info->int_sock);
    if (srv_info->cmd_socket == -1) {
        ret_val = -1;
        goto exit;
    }
    printf("cmd_socket successfully set: %d\n", srv_info->cmd_socket);

    /* Send empty session_uuid (0000) and recv actual session_uuid */
    ret_val = send_message(srv_info->cmd_socket, srv_info->session_uuid, UUID);
    if (ret_val == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to send message\n");
        goto exit;
    }
    ret_val = recv_message(srv_info->cmd_socket, buffer, UUID);
    if (ret_val == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to recv message\n");
        goto exit;
    }
    strncpy_s(srv_info->session_uuid, UUID, buffer, UUID);
    printf("UUID succesfully set: %s\n", srv_info->session_uuid);

exit:
    free(buffer);
    return ret_val;
}

/*  build_shell_socket
*
*
*
*/
int build_shell_socket(conn_info* srv_info)
{
    return 0;
}

/*  build_data_socket
*
*
*
*/
int build_data_socket(conn_info* srv_info)
{
    return 0;
}

/* remote_connect
* Does the leg work to build a socket with some basic error handling 
* 
* @char* srv_addr: The address to connect to
* @char* srv_port: The port to connect to
* @int int_sock: The type of socket to build (1: Stream; 2: DGRAM; 3: Raw)
* 
* @Returns: if successful, returns an int socket descriptor, otherwise -1
*/
int remote_connect(char* srv_addr, char* srv_port, int int_sock)
{
    WSADATA wsaData;

    /* Variables */
    int socket_descriptor = -1;
    int ret_val = -1;

    struct addrinfo hints, *addr_info, *AI;
    //struct socket socket_descriptor = INVALID_SOCKET; // incomplete type is not allowed?

    // Ask for Winsock
    if ((ret_val = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
        fprintf(stderr, "WSAStartup failed with error %d\n", ret_val);
        WSACleanup();
        return -1;
    }

    // Set the addrinfo struct to 0s
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags      = AI_NUMERICSERV; // Will not resolve server name
    hints.ai_flags     |= AI_NUMERICHOST; // Will not resolve host name
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = int_sock;      // 1: STREAM, 2: DGRAM, 3: RAW

    // Get address information
    ret_val = getaddrinfo(srv_addr, srv_port, &hints, &addr_info);
    if (ret_val != 0) {
        fprintf(stderr, "Cannot resolve address %s:%s, error %d\n", srv_addr, srv_port, ret_val);
        WSACleanup();
        return -1;
    }

    /* MAKE THIS A FUNCTION? -> Attempts to build connection, returns NULL if fails otherwise AI will have a valid struct*/
    // The Arrow(->) operator exists to access the members of the structure or the unions using pointers.
    // getaddrinfo returns one or more addrinfo structs, try each until success
    for (AI = addr_info; AI != NULL; AI = AI->ai_next) {

        // Open socket
        if ((socket_descriptor = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol)) < 0) {
            fprintf(stderr, "Building socket descriptor with socket() failed, error %d\n", socket_descriptor);
        }

        printf("Successful socket call > family: %d | socktype: %d | protocol: %d\n", AI->ai_family, AI->ai_socktype, AI->ai_protocol);

        // Establish connection to server (sets up remote mapping if DGRAM)
        // printf("Attempting to connect to %s:%s\n", srv_addr, srv_port);

        // If the connection fails, try the next addrinfo struct otherwise break
        if ((ret_val = connect(socket_descriptor, AI->ai_addr, AI->ai_addrlen)) < 0) {
            fprintf(stderr, "Connect() failed, error %d\n", ret_val);
            closesocket(socket_descriptor);
        } 
        else break;
    }
    /* MAKE THIS A FUNCTION END? */

    // Attempted all the addrinfo structs but made no valid connections
    if (AI == NULL) {
        fprintf(stderr, "Fatal: unable to connect to server.\n");
        WSACleanup();
        return -1;
    }

    // Finished with addrinfo chain
    freeaddrinfo(addr_info);

    get_connection_information(socket_descriptor, AI);

    return socket_descriptor;
}

/*  get_connection_information
* Provides various socket information like local & remote IP:PORT along with protocol family (IPv4/6)
*
* @int socket_descriptor: The socket to interact with
* @struct addrinfo* AI: The pointer to an addrinfo struct that will hold the relevent information
* 
* @returns: -1 on failure, 0 on success
*/
int get_connection_information(int socket_descriptor, struct addrinfo* AI)
{
    int ret_val;
    socklen_t addr_len;
    struct sockaddr_storage Addr;
    char addr_str[MAXADDRSIZE];

    addr_len = sizeof(Addr);
    if ((ret_val = getpeername(socket_descriptor, (struct sockaddr*)&Addr, &addr_len)) < 0) {
        fprintf(stderr, "getpeername() failed, error %d\n", ret_val);
        return -1;
    }
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len, addr_str, sizeof(addr_str), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("Connected to %s:%d via %s over %s\n",
            addr_str, ntohs(SS_PORT(&Addr)),
            (AI->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP",
            (AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
    }

    if ((ret_val = getsockname(socket_descriptor, (struct sockaddr*)&Addr, &addr_len)) < 0) {
        fprintf(stderr, "getsockname() failed with error %d\n", ret_val);
        return -1;
    }
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len, addr_str, sizeof(addr_str), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("Using local socket %s:%d\n",
            addr_str, ntohs(SS_PORT(&Addr)));
    }
    return 0;
}

/*  send_message
* Send data via a socket
* 
* @int socket_descriptor: The socket to be used
* @char* buffer: The data to be sent
* @size_t buffer_size: The size of the data to be sent
* 
* @Returns: -1 on failure, otherwise the number of bytes sent
*/
int send_message(int socket_descriptor, char* buffer, size_t buffer_size)
{
    int ret_val = 0;
    ret_val = send(socket_descriptor, buffer, buffer_size, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "send() failed with error %d\n", ret_val);
    }
    return ret_val;
}

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
        return recv_amount;
    }
    else if (0 == recv_amount) {
        printf("Recevied 0\n");
    }
    else {
        printf("Message: %s\n", buffer);
    }

    return recv_amount;
}

/*  cleanup_socket
* Shuts down, closes, and performs a WSACleanup on a socket
*
* @int socket_descriptor: The socket to be closed
* 
* Returns: 0... (not sure what to error handle in this)
*/
int cleanup_socket(int socket_descriptor)
{
    shutdown(socket_descriptor, SD_SEND);
    closesocket(socket_descriptor);
    WSACleanup();
    printf("Cleaned up Socket\n");
    return 0;
}

/*  recv_file
*
*
*
*/
int recv_file(int socket_descriptor) 
{
    int bytes_recvd = 0;
    char recv_buffer[BUFFER];
}