/***********************************************************/
/* Client_Utilities.c                                      */
/* Provides 'main' like functionaility to both Win and Nix */
/***********************************************************/

#include "Node_Utilities.h"
#include "Node_Validators.h"

unsigned int ERRORCODE = ERRORCODE_UNKNOWN;

// void (*client)(char*, unsigned short) 
// Points to a function (client) that takes two arugments (a char* and an unsigned short)

/* Takes user input and returns a struct of addr, port, protocol if successful otherwise NULL */
conn_info * menu(int argc, char* argv[])
{
    char* str_port;
    char* str_addr;
    char* str_sock;
    int socket_type;
    int int_port;

    conn_info* srv_info = malloc(sizeof(conn_info));

    JMP_IF(argc < 3, ERRORCODE_INPUT, FAIL, "Usage:%s {ip} {port}\n", argv[0]);

    str_addr = argv[1];
    str_port = argv[2];
    socket_type = SOCK_STREAM;

    JMP_IF(40 < sizeof(str_addr), ERRORCODE_INPUT, FAIL, "Invalid address: %s", str_addr);
    JMP_IF(5 < sizeof(str_port), ERRORCODE_INPUT, FAIL, "Invlaid Port: %s", str_port);

    int_port = validate_str_port(str_port); // Once the basic checks are passed, actually verify the port
    JMP_IF(-1 == int_port, ERRORCODE_INPUT, FAIL, "Invalid Port: %s", str_port);

    if (argc == 4) {
        str_sock = argv[3];
        if (!strcmp("UDP", str_sock)) {
            socket_type = SOCK_DGRAM;
        }
    }

    // Malloc and assign values to the struct
    srv_info = (struct conn_info *) malloc(sizeof(struct conn_info));
    JMP_IF(NULL == srv_info, ERRORCODE_ALLOCATE, FAIL, "menu: conn_info malloc failed - NULL ptr");

    strcpy_s(srv_info->addr, sizeof(srv_info->addr), str_addr);
    strcpy_s(srv_info->port, sizeof(srv_info->port), str_port);
    srv_info->socket_type = socket_type;
    strcpy_s(srv_info->session_uuid, sizeof(srv_info->session_uuid), "000000000-0000-0000-0000-000000000000");

    // Set default stuffs
    srv_info->cmd_socket = -1;
    srv_info->data_socket = -1;
    srv_info->shell_socket = -1;

    goto SUCCESS;

FAIL:
    printf("Failed to launch SPT Client! ERRORCODE: %d\n", ERRORCODE);
    if (NULL != srv_info) 
        free(srv_info);
    return NULL;

SUCCESS:
    printf("Launching SPT Client!\n");
    printf("Attempting to connect to %s:%d\n", str_addr, int_port);
    return srv_info;
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
    struct sockaddr_storage Addr = {0};
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

char encrypt(char reddata) {
    char blackdata;
    // do encryption here
    blackdata = reddata;
    return blackdata;
}
