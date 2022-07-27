/*                                                        */
/* Nix_Client.cpp                                         */
/* Provides IPv4 or IPv6 TCP connections for *Nix devices */
/*                                                        */

/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Client_Utilities.h"
#include "Client_Nix.h"

/* Constants */
#define MAXADDRSIZE 40 
#define BUFFER 256

int main(int argc, char* argv[])
{
    menu(argc, *argv, *client);
    exit(0);
}

int client(char* ip, unsigned short srv_port)
{
    /* Variables */
    int socket_descriptor = -1;
    int socket_descriptor_connection = -1;
    int reuse = 1;
    int recv_int;
    char recv_buffer[BUFFER];
    char addr_str[MAXADDRSIZE];

}