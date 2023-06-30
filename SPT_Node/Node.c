
/* Client.c */

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    #include "Platform_Win\\Node_Win.h"
#endif

#ifdef linux
    #include "Platform_Nix/Node_Nix.h"
#endif

#include "cpuid.h"

extern const char* ERRORSTR[];

int init(int argc, char* argv[], conn_info* srv_info)
{
    int ret_val = ERRORCODE_UNKNOWN;

    srv_info = menu(argc, argv);
    JMP_IF(NULL == srv_info, ERRORCODE, DONE, "main: Failed to get conn_info ptr from menu\n");

    ret_val = build_cmd_socket(srv_info);
    JMP_IF(-1 == ret_val, ERRORCODE_GENERIC, DONE, "main: build_cmd_socket failed\n");

DONE:
    return ret_val;
}

void deinit(conn_info* srv_info)
{
    if (NULL != srv_info) {
        if (-1 < srv_info->cmd_socket) {
            cleanup_socket(srv_info->cmd_socket);
        }
        if (-1 < srv_info->data_socket) {
            cleanup_socket(srv_info->data_socket);
        }
        if (-1 < srv_info->shell_socket) {
            cleanup_socket(srv_info->data_socket);
        }
    }
    free(srv_info);
}

void get_hostinfo()
{

    int cpuid_flags;
    char hostname[257] = { '\0' };

    printf("\nGathering host information:\n");
    cpuid_flags = get_cpuid_flags();
    printf("CPUID: %d\n", cpuid_flags);
    gethostname(hostname, 256);
    printf("Hostname: %s\n\n", hostname);

    void* buffer = malloc(UUID_SIZE);
    get_mac_address(buffer);

}

/* Not sure wtf I was thinking here, this should really just be a loop */
int main(int argc, char* argv[])
{
    int ret_val = 0;

    conn_info* srv_info = NULL;
    ret_val = init(argc, argv, srv_info);
    JMP_IF(ERRORCODE_SUCCESS != ret_val, ret_val, DONE, "Failed to init");

    get_hostinfo();

    while (true)
    {

    }

    /* Clean Exit */
DONE:
    deinit(srv_info);
    printf("Clean Exit! Closed with errorcode: %s\n", ERRORSTR[ERRORCODE]);
    exit(ret_val);
}
