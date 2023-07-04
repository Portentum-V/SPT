
#include "Node_Socket.h"

/* create_socket
* Does the leg work to build a socket with some basic error handling
*
* @char* srv_addr: The address to connect to
* @char* srv_port: The port to connect to
* @int sock_type: The type of socket to build (1: Stream; 2: DGRAM; 3: Raw)
*
* @Returns: if successful, returns an int socket descriptor, otherwise -1
*/
int create_socket(char* srv_addr, char* srv_port, int sock_type, int conn_type)
{
    // TODO: ifdef WINDOWS
    WSADATA wsaData;

    /* Variables */
    int socket_descriptor = -1;
    int ret_val = -1;
    const char sockopt = 1;

    struct addrinfo hints, * addr_info, * AI;
    //struct socket socket_descriptor = INVALID_SOCKET; // incomplete type is not allowed?

    // TODO: ifdef WINDOWS
    if ((ret_val = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
        fprintf(stderr, "WSAStartup failed with error %d\n", errno);
        WSACleanup();
        return -1;
    }

    // Set the addrinfo struct to 0s
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICSERV; // Will not resolve server name
    hints.ai_flags |= AI_NUMERICHOST; // Will not resolve host name
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;      // 1: STREAM, 2: DGRAM, 3: RAW

    // Get address information
    ret_val = getaddrinfo(srv_addr, srv_port, &hints, &addr_info);
    if (ret_val != 0) {
        fprintf(stderr, "Cannot resolve address %s:%s, error %d\n", srv_addr, srv_port, ret_val);
        // TODO: ifdef WINDOWS
        WSACleanup();
        return -1;
    }

    // The Arrow(->) operator exists to access the members of the structure or the unions using pointers.
    // getaddrinfo returns one or more addrinfo structs, try each until success
    for (AI = addr_info; AI != NULL; AI = AI->ai_next) {

        // Open socket
        if ((socket_descriptor = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol)) < 0) {
            fprintf(stderr, "Building socket descriptor with socket() failed, error %d\n", errno);
        }

        printf("Successful socket call > family: %d | socktype: %d | protocol: %d\n", AI->ai_family, AI->ai_socktype, AI->ai_protocol);

        if (SOCK_BIND == conn_type) {
            ret_val = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
            if (-1 == ret_val) {
                fprintf(stderr, "Setting SO_REUSEADDR failed, error: %d", errno);
                continue;
            }
        }

        // Establish connection to server (sets up remote mapping if DGRAM)
        // printf("Attempting to connect to %s:%s\n", srv_addr, srv_port);

        if (SOCK_CONN == conn_type && SOCK_DGRAM != sock_type) {
            // If the connection fails, try the next addrinfo struct otherwise break
            if ((ret_val = connect(socket_descriptor, AI->ai_addr, AI->ai_addrlen)) < 0) {
                fprintf(stderr, "Connect() failed, error %d\n", ret_val);
                closesocket(socket_descriptor);
            }
            else break;
        }
        /* Bind */
        else if (SOCK_BIND == conn_type) {
            ret_val = bind(socket_descriptor, AI->ai_addr, AI->ai_addrlen);
            if (0 != ret_val) {
                fprintf(stderr, "Connect() failed, error: %d\n", errno);
                closesocket(socket_descriptor);
            }
            else break;
        }
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

    printf("Valid socket created: %d\n", socket_descriptor);

    ret_val = get_connection_information(socket_descriptor);
    if (ret_val < 0) {
        // TODO: ifdef WINDOWS
        print_wsaerror();
    }

    return socket_descriptor;
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
    //TODO #ifdef WIN
    WSACleanup();
    printf("Cleaned up Socket\n");
    return 0;
}


/*  get_connection_information
* Provides various socket information like local & remote IP:PORT along with protocol family (IPv4/6)
*
* @int socket_descriptor: The socket to interact with
* @struct addrinfo* AI: The pointer to an addrinfo struct that will hold the relevent information
*
* @returns: -1 on failure, 0 on success
*/
int get_connection_information(int socket_descriptor)
{
    int ret_val = -1;
    uint32_t addr_len = 0;  //Alt for socklen_t without having to include network headers: unisgned opaque of at least 32 bits 
    struct sockaddr_storage Addr = { 0 };
    char remote_addr_str[MAXADDRSIZE];
    char remote_port_str[32];
    char local_addr_str[MAXADDRSIZE];
    char local_port_str[32];

    addr_len = sizeof(Addr);
    if ((ret_val = getpeername(socket_descriptor, (struct sockaddr*)&Addr, &addr_len)) < 0) {
        fprintf(stderr, "getpeername() failed, error %d\n", ret_val);
        return -1;
    }
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len,
            remote_addr_str, MAXADDRSIZE,
            remote_port_str, 32,
            NI_NUMERICHOST | NI_NUMERICSERV);
        /*
        printf("Connected to %s:%d via %s over %s\n",
            remote_addr_str, ntohs(SS_PORT(&Addr)),
            (AI->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP",
            (AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
        */
    }

    if ((ret_val = getsockname(socket_descriptor, (struct sockaddr*)&Addr, &addr_len)) < 0) {
        fprintf(stderr, "getsockname() failed with error %d\n", ret_val);
        return -1;
    }
    else {
        getnameinfo((struct sockaddr*)&Addr, addr_len,
            local_addr_str, MAXADDRSIZE,
            local_port_str, 32,
            NI_NUMERICHOST | NI_NUMERICSERV);
        /*
        printf("Using local socket %s:%d\n",
            local_addr_str, ntohs(SS_PORT(&Addr)));
        */
    }

    printf("local| %s:%s <---> %s:%s |remote\n",
        local_addr_str, local_port_str,
        remote_addr_str, remote_addr_str);

    return 0;
}


int echo_recv(int socket_descriptor)
{
    int ret_val = 0;
    char* buffer = malloc(10);
    if (buffer == NULL) {
        fprintf(stderr, "Malloc failed");
        ret_val = -1;
        goto DONE;
    }
    memset(buffer, 0, 10);

    ret_val = recv(socket_descriptor, buffer, 10, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "recv() failed with error %d\n", ret_val);
        goto DONE;
    }

    printf("\nRecv: %s\n", buffer);

    ret_val = send(socket_descriptor, buffer, 10, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "send() failed with error %d\n", ret_val);
        goto DONE;
    }

    printf("Sent: %s\n", buffer);

DONE:
    return ret_val;
}