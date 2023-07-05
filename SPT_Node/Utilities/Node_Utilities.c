/***********************************************************/
/* Client_Utilities.c                                      */
/* Provides 'main' like functionaility to both Win and Nix */
/***********************************************************/

#include "Node_Utilities.h"

void sleep_ms(int milliseconds)
{
#ifdef OS_WIN
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}
// void (*client)(char*, unsigned short) 
// Points to a function (client) that takes two arugments (a char* and an unsigned short)

void get_hostinfo(void)
{

    int cpuid_flags;
    char hostname[257] = { '\0' };

    printf("\nGathering host information:\n");

    cpuid_flags = get_cpuid_flags();
    printf("CPUID: %d\n", cpuid_flags);

#ifdef OS_WIN
    JMP_IF(ERRORCODE_SUCCESS != start_wsa(), ERRORCODE_WSA, DONE);
#endif

    gethostname(hostname, 256);
    printf("Hostname: %s\n\n", hostname);

    void* buffer = malloc(UUID_SIZE);
    get_mac_address(buffer);
    print_wsaerror();

DONE:
    return;
}