/***********************************************************/
/* Client_Utilities.c                                      */
/* Provides 'main' like functionaility to both Win and Nix */
/***********************************************************/

#include "Node_Utilities.h"

struct ulid_generator* g_ulid_gen = NULL;

int init_ulid(void)
{
    /* ulid generator already ready */
    RET_IF(NULL != g_ulid_gen, ERRORCODE_REINIT, ERRORCODE_REINIT);

    g_ulid_gen = calloc(1, sizeof(struct ulid_generator));
    RET_IF(NULL == g_ulid_gen, ERRORCODE_ALLOCATE, ERRORCODE_ALLOCATE);

    /* 1 = userspace entropy, 0 = system entropy */
    return ulid_generator_init(g_ulid_gen, ULID_RELAXED);
}

void sleep_ms(int milliseconds)
{
#ifdef OS_WIN
    Sleep(milliseconds);
    // TODO: Move POSIX check to Node_OS?
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

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
