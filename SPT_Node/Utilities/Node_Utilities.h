/***********************************************************/
/* Node_Utilities.h                                      */
/***********************************************************/

#pragma once
/* Headers */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    #include <winsock2.h>
    #include <ws2tcpip.h>              /* Provides getnameinfo for get_connection_information */
    #pragma comment(lib, "Ws2_32.lib") /* Provides ws2tcpip.h */
#endif

#include "..\\Error\\Node_Error.h"

#include "Node_Converters.h"
#include "Node_Validators.h"

/* Constants */
#define HWADDRSIZE 13
#define MAXADDRSIZE 40
#define UUID_SIZE 38
#define UUID_NONE "00000000-0000-0000-0000-000000000000"

#define BUFFER 256

/* Functions */
void sleep_ms(int milliseconds);