/***********************************************************/
/* Client_Utilities.c                                      */
/* Provides 'main' like functionaility to both Win and Nix */
/***********************************************************/

/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
struct conn_info * menu(int argc, char* argv[])
{
    char* str_port;
    char* str_addr;
    char* str_sock;
    int int_sock;
    int int_port;

    struct conn_info *srv_info;
    srv_info = NULL;

    if (argc < 3) {
        printf("Usage: %s {ip} {port}\n", argv[0]);
        return NULL;
    }

    str_addr = argv[1];
    str_port = argv[2];
    int_sock = SOCK_STREAM;

    if (40 < sizeof(str_addr)) {
        fprintf(stderr, "Invalid address: %s", str_addr);
        return NULL;
    }

    if (5 < sizeof(str_port)) {
        fprintf(stderr, "Invalid Port: %s", str_port);
        return NULL;
    }

    int_port = check_port(str_port); // Once the basic checks are passed, actually verify the port
    if (-1 == int_port) {
        fprintf(stderr, "Invalid Port: %s", str_port);
        return NULL;
    }

    if (argc == 4) {
        str_sock = argv[3];
        if (!strcmp("UDP", str_sock)) {
            int_sock = SOCK_DGRAM;
        }
    }

    // Malloc and assign values to the struct
    srv_info = (struct conn_info *) malloc(sizeof(struct conn_info));
    if (srv_info == NULL) {
        fprintf(stderr, "malloc() failed.\n");
        return NULL;
    }
    strcpy_s(srv_info->srv_addr, sizeof(srv_info->srv_addr), str_addr);
    strcpy_s(srv_info->srv_port, sizeof(srv_info->srv_port), str_port);
    srv_info->int_sock = int_sock;

    printf("Launching SPT Client!\n");
    printf("Attempting to connect to %s:%d...\n", str_addr, int_port);
    return srv_info;
}

char encrypt(char reddata) {
    char blackdata;
    // do encryption here
    blackdata = reddata;
    return blackdata;
}