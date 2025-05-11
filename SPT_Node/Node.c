
/* Node.c */

#include "Utilities/Node_Utilities.h"
#include "Networking/Node_Networking.h"
#include "Networking/Node_Connection.h"
#include "Configuration/Node_Config.h"

extern const char* ERRORSTR[];

int static deinit(conn_info* srv_info)
{
    // deinit_conn_struct(srv_info);
	g_error = ERRORCODE_SUCCESS;
    return 0;
}

int static init(int level, int depth, char* str_port, char* str_addr, int sock_type, conn_info** srv_info)
{
    int ret_val = ERRORCODE_UNKNOWN;

    ret_val = init_error(level, depth);
    ret_val = init_ulid();

    log_debug("init_conn_struct");
    *srv_info = init_conn_struct(str_addr, str_port, sock_type);
    JMP_PRINT_IF(NULL == *srv_info, g_error, DONE, "main: Failed to get conn_info ptr from menu\n");

    ret_val = build_cmd_socket(*srv_info);
    JMP_PRINT_IF(-1 == ret_val, ERRORCODE_GENERIC, DONE, "main: build_cmd_socket failed\n");

DONE:
    if (ERRORCODE_SUCCESS != ret_val) {
        deinit(*srv_info);
        *srv_info = NULL;
    }
    return ret_val;
}

uint16_t static menu(int argc, char* argv[], 
              int* level, int* depth,
              char**  str_port, char**  str_addr, int* sock_type)
{
    char* str_sock;
    int i = 0;

    log_trace("argc: %d\n", argc);

    RET_PRINT_IF(argc < 5, ERRORCODE_INPUT, ERRORCODE_INPUT, 
           "Usage:%s {loglevel} {folderdepth} {ip} {port} {[UDP]}\n", argv[0]);

    // T0DO if debug?
    for (i; i < argc; ++i) {
        log_trace("[%d]: %s\n", i, argv[i]);
    }

    char** tmp_level = NULL;
    *level = strtol(argv[1], tmp_level, 10);

    char** tmp_depth = NULL;
    *depth = strtol(argv[2], tmp_depth, 10);

    *str_addr = argv[3];
    RET_PRINT_IF(40 < sizeof(str_addr), ERRORCODE_INPUT, ERRORCODE_INPUT, "Invalid address: %s", *str_addr);

    *str_port = argv[4];
    RET_PRINT_IF(-1 == convert_port(*str_port), ERRORCODE_INPUT, ERRORCODE_INPUT, "Invalid Port: %s", *str_port);

    *sock_type = SOCK_STREAM;
    if (argc == 6) {
        str_sock = argv[5];
        if (!strcmp("UDP", str_sock)) {
            *sock_type = SOCK_DGRAM;
        }
    }

    log_info("Connecting to %s:%s|%s\n", *str_addr, *str_port, SOCKSTR[*sock_type]);
    return ERRORCODE_SUCCESS;
}

void execute(void)
{
    log_debug("A thread eventually...\n");
    sleep_ms(1000);
    g_loop = false;
    return;
}

int main(int argc, char* argv[])
{
    int ret_val = 0;
    conn_info* srv_info = NULL;

    char* str_port = NULL;
    char* str_addr = NULL;
    int sock_type = SOCK_BAD;
    int level = 0;
    int depth = 5;

    ret_val = menu(argc, argv,
                   &level, &depth,
                   &str_port, &str_addr, &sock_type);
    JMP_PRINT_IF(ERRORCODE_SUCCESS != ret_val, ret_val, DONE, "Failed to init\n")

    ret_val = init(level, depth, str_port, str_addr, sock_type, &srv_info);
    JMP_PRINT_IF(ERRORCODE_SUCCESS != ret_val, ret_val, DONE, "Failed to init\n")

    log_info("Launching SPT Node!\n");

    get_hostinfo(); /* Some sort of 'startup' function to call back? */
    while (g_loop)
    {
        execute();
    }

    /* Clean Exit */
DONE:
    ret_val = deinit(srv_info);
    log_info("Clean Exit! Closed with errorcode: %s\n", ERRORSTR[g_error]);
    exit(ret_val);
}
