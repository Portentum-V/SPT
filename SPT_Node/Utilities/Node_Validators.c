
/***********************************************************/
/* Node_Validators.c                                       */
/* Provides 'main' like functionaility to both Win and Nix */
/***********************************************************/

#include "Node_Validators.h"

/* Validates Port, returns -1 if Invalid */
//unsigned short check_port(char* input)
int validate_str_port(char* input)
{
    long port;
    char* tmp_ptr;
    port = strtol(input, &tmp_ptr, 10);
    if (port >= 0 && port <= 65535) {
        return (unsigned short)port;
    }
    else {
        return -1;
    }
}
