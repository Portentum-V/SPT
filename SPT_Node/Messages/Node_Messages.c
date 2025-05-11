
/* Client_Messages.c - Nano Protocol Buffers*/

#include "Node_Messages.h"

static bool write_callback(pb_ostream_t* stream, const uint8_t* buf, size_t count)
{
    int fd = (intptr_t)stream->state;
    return send(fd, buf, count, 0) == count;
}

static bool read_callback(pb_istream_t* stream, uint8_t* buf, size_t count)
{
    int fd = (intptr_t)stream->state;
    int result;

    if (count == 0)
        return true;

    result = recv(fd, buf, count, MSG_WAITALL);

    if (result == 0)
        stream->bytes_left = 0; /* EOF */

    return result == count;
}

pb_ostream_t pb_ostream_from_socket(int fd)
{
    pb_ostream_t stream = { &write_callback, (void*)(intptr_t)fd, SIZE_MAX, 0 };
    return stream;
}

pb_istream_t pb_istream_from_socket(int fd)
{
    pb_istream_t stream = { &read_callback, (void*)(intptr_t)fd, SIZE_MAX };
    return stream;
}

// Initializes route header with CRC and ULID?
SPT_Route * init_route_header(uint8_t priority, uint8_t hopcount, 
                              pb_byte_t *source, pb_byte_t *destination)
{
    errorcode error = ERRORCODE_UNKNOWN;
    SPT_Route * route;
    route = (SPT_Route *)calloc(1, sizeof(SPT_Route));
    RET_IF(NULL == route, ERRORCODE_ALLOCATE, NULL)

    route->priority = priority;
    route->hopcount = hopcount;

    memcpy(route->source, source, ID_SIZE);
    JMP_IF(NULL == route->source, ERRORCODE_ALLOCATE, DONE)
    memcpy(route->destination, destination, ID_SIZE);
    JMP_IF(NULL == route->destination, ERRORCODE_ALLOCATE, DONE)

    unsigned char* ulid = ulid_generate_binary(g_ulid_gen);
    memcpy(route->ulid, ulid, ID_SIZE);

DONE:
    // TODO: Is this safe? Probably not
    if (g_error != ERRORCODE_SUCCESS)
    {
        if (NULL != route) {
            free(route);
            route = NULL;
        }
    }
    return route;
}
