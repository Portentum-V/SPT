
/* Node_Converters.c */

#include "Node_Converters.h"

/* Converts Str port to int, returns -1 if Invalid */
//unsigned short check_port(char* input)
int convert_port(char* str_port)
{
    if (5 < strlen(str_port)) {
        return -1;
    }

    long port = -1;
    char** tmp_ptr = NULL;
    port = strtol(str_port, tmp_ptr, 10);
    if (port >= 0 && port <= 65535) {
        return (unsigned short)port;
    }
    else {
        g_error = ERRORCODE_INVALID;
        return -1;
    }
}
