
/* Node.c */

#include "Utilities\\Node_Utilities.h"
#include "Networking\\Node_Networking.h"
#include "Configuration\\Node_Config.h"

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    /* Win */
    #include "Platform_Win\\Node_Win.h"
#endif

#if LINUX
    /* Nix */
    #include "Platform_Nix\\Node_Nix.h"

#endif

#include "cpuid.h"

extern const char* ERRORSTR[];

void deinit(conn_info* srv_info)
{
    deinit_conn_struct(srv_info);
}

int init(char* str_port, char* str_addr, int sock_type, conn_info** srv_info)
{
    int ret_val = ERRORCODE_UNKNOWN;

    ret_val = init_error(0);

    log_debug("init_conn_struct");
    *srv_info = init_conn_struct(str_addr, str_port, sock_type);
    JMP_IF(NULL == *srv_info, g_error, DONE, "main: Failed to get conn_info ptr from menu\n");

    ret_val = build_cmd_socket(*srv_info);
    JMP_IF(-1 == ret_val, ERRORCODE_GENERIC, DONE, "main: build_cmd_socket failed\n");

DONE:
    if (ERRORCODE_SUCCESS != ret_val) {
        deinit(*srv_info);
    }
    return ret_val;
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

int menu(int argc, char* argv[], 
         char**  str_port, char**  str_addr, int* sock_type)
{
    char* str_sock;
    int i = 0;

    printf("argc: %d\n", argc);

    RET_IF(argc < 3, ERRORCODE_INPUT, ERRORCODE_INPUT, "Usage:%s {ip} {port}\n", argv[0]);

    printf("Arguments:\n");
    for (i; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

    *str_addr = argv[1];
    RET_IF(40 < sizeof(str_addr), ERRORCODE_INPUT, ERRORCODE_INPUT, "Invalid address: %s", *str_addr);

    *str_port = argv[2];
    RET_IF(-1 == convert_port(*str_port), ERRORCODE_INPUT, ERRORCODE_INPUT, "Invalid Port: %s", *str_port);

    *sock_type = SOCK_STREAM;
    if (argc == 4) {
        str_sock = argv[3];
        if (!strcmp("UDP", str_sock)) {
            *sock_type = SOCK_DGRAM;
        }
    }

    printf("Connecting to %s:%s|%d\n", *str_addr, *str_port, *sock_type);
    return ERRORCODE_SUCCESS;
}

void execute()
{
    printf("A thread eventually...\n");
    sleep_ms(1000);
    g_loop = false;
    return;
}

int main(int argc, char* argv[])
{
    printf("Test!\n");
    int ret_val = 0;
    conn_info* srv_info = NULL;

    char* str_port = NULL;
    char* str_addr = NULL;
    int sock_type = SOCK_BAD;

    ret_val = menu(argc, argv, &str_port, &str_addr, &sock_type);
    JMP_IF(ERRORCODE_SUCCESS != ret_val, ret_val, DONE, "Failed to init");

    ret_val = init(str_port, str_addr, sock_type, &srv_info);
    JMP_IF(ERRORCODE_SUCCESS != ret_val, ret_val, DONE, "Failed to init");

    printf("Launching SPT Node!\n");
    get_hostinfo();

    while (g_loop)
    {
        execute();
    }

    /* Clean Exit */
DONE:
    deinit(srv_info);
    printf("Clean Exit! Closed with errorcode: %s\n", ERRORSTR[g_error]);
    exit(ret_val);
}
