/***********************************************************/
/* Node_Win.h                                            */
/***********************************************************/

#pragma once

#include "..\\Utilities\\Node_Utilities.h"

/* Threading */
// #include <windows.h> /* MUST come after winsock2 (includes winsock?)*/

/* IP_ADAPTER_INFO && PIP_ADAPTER_INFO */
#include <iphlpapi.h> 
#pragma comment(lib, "IPHLPAPI.lib")

errorcode get_mac_address(char *network_address);

void print_wsaerror();
