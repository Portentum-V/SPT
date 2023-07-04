/* Node_Validators.c */

#include "Node_Validators.h"


uint16_t validate_port(uint16_t port)
{
    return validate_range((uint32_t)port, 0, 65535);
}

uint16_t validate_range(uint32_t num, uint32_t min, uint32_t max)
{
    if (num >= min && num <= max) {
        return ERRORCODE_SUCCESS;
    }
    return ERRORCODE_INVALID;
}