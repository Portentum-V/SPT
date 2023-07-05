/***********************************************************/
/* Node_Win.h                                            */
/***********************************************************/

#pragma once

#include "..\\Utilities\\Node_Utilities.h"

/* Networking */
#include <winsock2.h>
#include <ws2tcpip.h>              /* Provides getnameinfo for get_connection_information */
#pragma comment(lib, "Ws2_32.lib") /* Provides ws2tcpip.h */

/* IP_ADAPTER_INFO && PIP_ADAPTER_INFO */
#include <iphlpapi.h> 
#pragma comment(lib, "IPHLPAPI.lib")

/* Threading */
// #include <windows.h> /* MUST come after winsock2 (includes winsock?)*/

/* Functions */
errorcode start_wsa(void);

void print_wsaerror(void);

errorcode get_mac_address(char *network_address);
