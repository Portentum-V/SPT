/***********************************************************/
/* Client_Utilities.c                                      */
/* Provides 'main' like functionaility to both Win and Nix */
/***********************************************************/

#include "Client_Utilities.h"

/* Validates Port, returns -1 if Invalid */
//unsigned short check_port(char* input)
int check_port(char* input)
{
    long port;
    char* tmp_ptr;
    port = strtol(input, &tmp_ptr, 10);
    if (port >= 0 && port <= 65535) {
        return (unsigned short)port;
    }
    else {
        /* printf("Thats not really a vaild port... but I'll hook you up!\n"); */
        return -1;
    }
}

// void (*client)(char*, unsigned short) 
// Points to a function (client) that takes two arugments (a char* and an unsigned short)

/* Takes user input and returns a struct of addr, port, protocol if successful otherwise NULL */
conn_info * menu(int argc, char* argv[])
{
    char* str_port;
    char* str_addr;
    char* str_sock;
    int int_sock;
    int int_port;

    conn_info* srv_info = malloc(sizeof(conn_info));

    FAIL_IF_JMP(argc < 3, NULL, "Usage:%s {ip} {port}\n", argv[0]);
    /* 
    if (argc < 3) {
        printf("Usage: %s {ip} {port}\n", argv[0]);
        return NULL;
    } */

    str_addr = argv[1];
    str_port = argv[2];
    int_sock = SOCK_STREAM;

    FAIL_IF_JMP(40 < sizeof(str_addr), NULL, "Invalid address: %s", str_addr);
    /*
    if (40 < sizeof(str_addr)) {
        fprintf(stderr, "Invalid address: %s", str_addr);
        return NULL;
    } */

    FAIL_IF_JMP(5 < sizeof(str_port), NULL, "Invlaid Port: %s", str_port);
    /*
    if (5 < sizeof(str_port)) {
        fprintf(stderr, "Invalid Port: %s", str_port);
        return NULL;
    }*/

    int_port = check_port(str_port); // Once the basic checks are passed, actually verify the port
    FAIL_IF_JMP(-1 == int_port, NULL, "Invalid Port: %s", str_port);
    /*
    if (-1 == int_port) {
        fprintf(stderr, "Invalid Port: %s", str_port);
        return NULL;
    } */

    if (argc == 4) {
        str_sock = argv[3];
        if (!strcmp("UDP", str_sock)) {
            int_sock = SOCK_DGRAM;
        }
    }

    // Malloc and assign values to the struct
    srv_info = (struct conn_info *) malloc(sizeof(struct conn_info));
    FAIL_IF_JMP(NULL == srv_info, ERRORCODE_ALLOCATE, "menu: conn_info malloc failed - NULL ptr");
    /*
    if (srv_info == NULL) {
        fprintf(stderr, "malloc() failed.\n");
        return NULL;
    } */

    strcpy_s(srv_info->srv_addr, sizeof(srv_info->srv_addr), str_addr);
    strcpy_s(srv_info->srv_port, sizeof(srv_info->srv_port), str_port);
    srv_info->int_sock = int_sock;
    strcpy_s(srv_info->session_uuid, sizeof(srv_info->session_uuid), "000000000-0000-0000-0000-000000000000");

    // Set default stuffs
    srv_info->cmd_socket = -1;
    srv_info->data_socket = -1;
    srv_info->shell_socket = -1;

    goto SUCCESS;

FAIL:
    printf("Failed to launch SPT Client! ERRORCODE: %d", errorcode);
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
    struct sockaddr_storage Addr;
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

int buffer_append(void **buffer,
                  size_t *buffer_len, size_t *buffer_size,
                  void *data, size_t data_len)
{
    void *new_buffer = NULL;
    size_t need = *buffer_len + data_len;
    if (need > *buffeR_size) {
        new_buffer = realloc(*buffer, need);
        FAIL_IF_RET(NULL == new_buffer, -1, "Failed to realloc for buffer resize\n");
        *buffer = new_buffer;
    }
    
    memcpy(*buffer + *buffer_len, data, data_len);
    *buffer_len += data_len;
    return 0;
}
        
