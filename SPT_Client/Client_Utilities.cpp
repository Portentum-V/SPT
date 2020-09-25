/*                                                         */
/* Client_Utilities.cpp                                    */
/* Provides 'main' like functionaility to both Win and Nix */
/*                                                         */

/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Socket types. */
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			        */

/* Validates Port, returns 65535 if Invalid */
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

/* Essentially 'Main' but didn't want to write the code twice */
int menu(int argc, char* argv[], int (*client)(char*, char*, int))
{
    char* str_port;
    char* srv_addr;
    char* str_sock;
    int sock_type;
    int int_port;

    if (argc < 3) {
        printf("Usage: %s {ip} {port}\n", argv[0]);
        return -1;
    }

    srv_addr = argv[1];
    str_port = argv[2];
    sock_type = SOCK_STREAM;
    int_port = check_port(str_port);

    if (-1 == int_port) {
        fprintf(stderr, "Invalid Port: %s", str_port);
        return -1;
    }

    if (argc == 4) {
        str_sock = argv[3];
        if (!strcmp("UDP", str_sock)) {
            sock_type = SOCK_DGRAM;
        }
    }

    printf("Launching SPT Client!\n");
    printf("Attempting to connect to %s on %d...\n", srv_addr, int_port);
    client(srv_addr, str_port, sock_type);
    return 1;
}

char encrypt(char reddata) {
    char blackdata;
    // do encryption here
    blackdata = reddata;
    return blackdata;
}