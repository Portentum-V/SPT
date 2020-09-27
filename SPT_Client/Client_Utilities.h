/***********************************************************/
/* Client_Utilities.h                                      */
/***********************************************************/

#pragma once

/* Constants */
#define MAXADDRSIZE 40 
#define BUFFER 256

/* Socket types. */
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			        */

/* Structs */
struct conn_info {
    char srv_addr[MAXADDRSIZE];
    char srv_port[5];
    int int_sock;
};

/* Utility functions */
int check_port(char* input);
struct conn_info* menu(int argc, char* argv[]);

//int menu(int argc, char* argv[], int (*client)(char*, char*, int)); // Switch to passing a struct in the event the data is useful again.
