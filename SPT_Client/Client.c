/**********************************************************/
/* Client.c                                               */
/*														  */
/**********************************************************/

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    #include "Client_Win.h" // Windows
#endif

#ifdef linux
    //*Nix
#endif

#include "cpuid.h"

int main(int argc, char* argv[])
{
    int ret_val = 0;
    //int i = 0;
    void* buffer = malloc(UUID);

    int cpuid_flags;
    char hostname[257] = { '\0' };

    conn_info* srv_info = NULL;
    /*
    conn_info* srv_info = malloc(sizeof(conn_info));
    if (srv_info == NULL) {
        fprintf(stderr, "Failed to malloc conn_info\n");
        ret_val = -1;
        goto exit;
    } 
    memset(srv_info, 0, sizeof(conn_info)); */

    srv_info = menu(argc, argv);
    FAIL_IF_JMP(NULL == srv_info, ERRORCODE_ALLOCATE, "main: Failed to get conn_info ptr from menu\n");
    /*
    if ((srv_info = menu(argc, argv)) == NULL) {
        fprintf(stderr, "Failed to get input from menu\n");
        ret_val = -1;
        goto exit;
    } */

    ret_val = build_cmd_socket(srv_info);
    FAIL_IF_JMP(-1 == ret_val, ERRORCODE_GENERIC, "main: build_cmd_socket failed\n");
    /*
    if (ret_val == -1) {
        fprintf(stderr, "build_cmd_socket failed\n");
        goto exit;
    } */

    cpuid_flags = get_cpuid_flags();
    printf("\nCPUID: %d\n", cpuid_flags);
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
FAIL:
exit:
    if (srv_info != NULL) {
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
    printf("Clean Exit\n");
    exit(ret_val);
}