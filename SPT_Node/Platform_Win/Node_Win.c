/**********************************************************/
/* Node_Win.c                                           */
/* Provides IPv4 and IPv6 connections for Win devices     */
/**********************************************************/

#include "Node_Win.h"

errorcode start_wsa(void)
{
    WSADATA wsaData;
    int ret_val = WSAStartup(MAKEWORD(1, 1), &wsaData);

    if (0 == ret_val) { 
        return ERRORCODE_SUCCESS; 
    }

    log_warn("WSAStartup failed with error %d\n", errno);
    WSACleanup();
    return ERRORCODE_WSA;
}

void print_wsaerror(void)
{
    wchar_t* s = NULL;
    long dwMessageId = WSAGetLastError();
	if (0 == dwMessageId) {
		return;
	}

    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, dwMessageId,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPWSTR)&s, 0, NULL);
    log_error("Id %ld; Message: %S\n", dwMessageId, s);
    LocalFree(s);
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
errorcode get_mac_address(char *network_address)
{
    uint16_t ret_val = ERRORCODE_SUCCESS;
    unsigned int i = 0;
    char hw_str[HWADDRSIZE] = { 0 };
    char two_bytes[3] = { 0 };

    socklen_t addr_len = 0;
    char addr_str[MAXADDRSIZE] = { 0 };
    struct sockaddr_storage addr = { 0 };
    addr_len = sizeof(addr);

    ULONG flags = 0x0010;     /* GAA_FLAG_INCLUDE_PREFIX (Show IPv4 and IPv6) */
    ULONG family = AF_UNSPEC; /* 0 */

    PIP_ADAPTER_ADDRESSES ptr_addresses = NULL;
    ULONG out_buf_len = 0;

    PIP_ADAPTER_ADDRESSES ptr_current_addresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS ptr_unicast_address = NULL;
    PIP_ADAPTER_ADDRESSES tmp = NULL;

    out_buf_len = sizeof(IP_ADAPTER_ADDRESSES);
    ptr_addresses = (PIP_ADAPTER_ADDRESSES)malloc(out_buf_len); 

    JMP_IF(ERRORCODE_SUCCESS != start_wsa(), ERRORCODE_WSA, EXIT);

    /* Ensure ptr_addresses size is correct */
    if (GetAdaptersAddresses(family, flags, NULL, ptr_addresses, &out_buf_len) == ERROR_BUFFER_OVERFLOW) {
        if (NULL != ptr_addresses) { 
            free(ptr_addresses);
            ptr_addresses = NULL;
        }
        tmp = realloc(ptr_addresses, out_buf_len);
        if (NULL == tmp) {
            ret_val = ERRORCODE_ALLOCATE;
            log_error("Failed to malloc/realloc for GetAdaptersAddresses(), error: %lu", ERROR_BUFFER_OVERFLOW);
            goto EXIT;
        }
        ptr_addresses = tmp;
    }

    log_trace("Memory allocated for GetAdapterAddresses = %ld bytes\n", out_buf_len);
    /* Actually call GetAdaptersAddresses for data */
    ret_val = GetAdaptersAddresses(family, flags, NULL, ptr_addresses, &out_buf_len);
    if (NO_ERROR == ret_val) { /* NO_ERROR == 0 */
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
                for (i = 0; i < ptr_current_addresses->PhysicalAddressLength; i++) {
                    if (i == (ptr_current_addresses->PhysicalAddressLength - 1)) {
                        sprintf_s(two_bytes, 3, "%.2X\0", (int)ptr_current_addresses->PhysicalAddress[i]);
                    }
                    else {
                        sprintf_s(two_bytes, 3, "%.2X", (int)ptr_current_addresses->PhysicalAddress[i]);
                    }
                    strncat_s(hw_str, HWADDRSIZE, two_bytes, 2);
                }
                printf("%s\n", hw_str);
                memset(hw_str, 0, HWADDRSIZE);
            }
            ptr_current_addresses = ptr_current_addresses->Next;
        }
    }
    else {
        log_error("GetAdapterAddresses failed, error: %d", ret_val);
        ret_val = ERRORCODE_FAILED;
    }

EXIT:
    if (NULL != ptr_addresses) {
        free(ptr_addresses);
    }
    return ret_val;
}
