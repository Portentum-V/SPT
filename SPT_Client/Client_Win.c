/**********************************************************/
/* Client_Win.c                                           */
/* Provides IPv4 or IPv6 TCP connections for *Nix devices */
/**********************************************************/

/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "Client_Utilities.h"

// Link with ws2_32.lib <- Not sure if this is required? -> It is...
#pragma comment(lib, "Ws2_32.lib")

int client(char* srv_addr, char* srv_port, int int_sock);

int main(int argc, char* argv[])
{
    struct conn_info* srv_info;

    if ((srv_info = menu(argc, argv)) == NULL) {
        exit(1);
    }

    client(srv_info->srv_addr, srv_info->srv_port, srv_info->int_sock);
    //Do stuff and things

    /* Clean Exit */
    free(srv_info);
    printf("Clean Exit\n");
    exit(0);
}

int client(char* srv_addr, char* srv_port, int int_sock)
{

    WSADATA wsaData;

    /* Variables */
    int socket_descriptor = -1;
    int ret_val = -1;
    int addr_len = 0;

    char addr_str[MAXADDRSIZE];

    struct addrinfo hints, *addr_info, *AI;
    //struct socket socket_descriptor = INVALID_SOCKET; // incomplete type is not allowed?
    struct sockaddr_storage Addr;

    //struct sockaddr_in6 server_in, client_in;
    //socklen_t addr_size = sizeof(client_in);

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

    /* MAKE THIS A FUNCTION? -> Prints connection information*/
    addr_len = sizeof(Addr);
    if ((ret_val = getpeername(socket_descriptor, (struct sockaddr*)&Addr, &addr_len)) < 0) {
        fprintf(stderr, "getpeername() failed, error %d\n", ret_val);
    } 
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len, addr_str, sizeof(addr_str), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("Connected to %s:%d via protocol %s on protocol family %s\n",
            addr_str, ntohs(SS_PORT(&Addr)),
            (AI->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP",
            (AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
    }

    // Finished with addrinfo chain
    freeaddrinfo(addr_info);
        
    if ((ret_val = getsockname(socket_descriptor, (struct sockaddr*)&Addr, &addr_len)) < 0) {
        fprintf(stderr, "getsockname() failed with error %d\n", ret_val);
    }
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len, addr_str, sizeof(addr_str), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("Using local socket %s:%d\n",
            addr_str, ntohs(SS_PORT(&Addr)));
    }
    /* MAKE THIS A FUNCTION END? */

    // Clean up and return
    shutdown(socket_descriptor, SD_SEND);
    closesocket(socket_descriptor);
    WSACleanup();
    printf("Cleaned up Socket\n");
    return 0;
}
/*
int recv_file(int socket_descriptor) 
{
    int bytes_recvd = 0;
    char recv_buffer[BUFFER];
}
*/