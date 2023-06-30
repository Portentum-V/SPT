/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_MESSAGE_PB_H_INCLUDED
#define PB_MESSAGE_PB_H_INCLUDED
#include "..\\nanopb\\pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
/* Size: 46 bytes */
typedef struct _SPT_Route {
    /* Routing */
    uint8_t priority;
    uint8_t hopcount;
    pb_byte_t source[16];
    pb_byte_t destination[16];
    uint64_t session;
    /* Validate (excludes hopcount) */
    uint32_t crc;
} SPT_Route;

/* Size: 23 bytes (+ Payload) */
typedef struct _SPT_Task {
    /* Request or Results */
    bool type;
    /* Task UUID: Reused if fragmented */
    pb_byte_t id[16];
    /* RPC */
    uint16_t module;
    uint16_t method;
    uint16_t status;
    /* Payload
 If size is > payload, expect more messages with same id */
    uint64_t size;
    pb_callback_t payload;
} SPT_Task;

typedef struct _SPT_Header {
    /* Protocol Version */
    uint8_t version;
    bool has_route;
    SPT_Route route;
    /* One message may contain multiple tasks */
    pb_callback_t task;
} SPT_Header;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define SPT_Route_init_default                   {0, 0, {0}, {0}, 0, 0}
#define SPT_Task_init_default                    {0, {0}, 0, 0, 0, 0, {{NULL}, NULL}}
#define SPT_Header_init_default                  {0, false, SPT_Route_init_default, {{NULL}, NULL}}
#define SPT_Route_init_zero                      {0, 0, {0}, {0}, 0, 0}
#define SPT_Task_init_zero                       {0, {0}, 0, 0, 0, 0, {{NULL}, NULL}}
#define SPT_Header_init_zero                     {0, false, SPT_Route_init_zero, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define SPT_Route_priority_tag                   1
#define SPT_Route_hopcount_tag                   2
#define SPT_Route_source_tag                     3
#define SPT_Route_destination_tag                4
#define SPT_Route_session_tag                    5
#define SPT_Route_crc_tag                        6
#define SPT_Task_type_tag                        1
#define SPT_Task_id_tag                          2
#define SPT_Task_module_tag                      3
#define SPT_Task_method_tag                      4
#define SPT_Task_status_tag                      5
#define SPT_Task_size_tag                        6
#define SPT_Task_payload_tag                     7
#define SPT_Header_version_tag                   1
#define SPT_Header_route_tag                     2
#define SPT_Header_task_tag                      3

/* Struct field encoding specification for nanopb */
#define SPT_Route_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   priority,          1) \
X(a, STATIC,   SINGULAR, UINT32,   hopcount,          2) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, source,            3) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, destination,       4) \
X(a, STATIC,   SINGULAR, UINT64,   session,           5) \
X(a, STATIC,   SINGULAR, UINT32,   crc,               6)
#define SPT_Route_CALLBACK NULL
#define SPT_Route_DEFAULT NULL

#define SPT_Task_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     type,              1) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, id,                2) \
X(a, STATIC,   SINGULAR, UINT32,   module,            3) \
X(a, STATIC,   SINGULAR, UINT32,   method,            4) \
X(a, STATIC,   SINGULAR, UINT32,   status,            5) \
X(a, STATIC,   SINGULAR, UINT64,   size,              6) \
X(a, CALLBACK, SINGULAR, BYTES,    payload,           7)
#define SPT_Task_CALLBACK pb_default_field_callback
#define SPT_Task_DEFAULT NULL

#define SPT_Header_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   version,           1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  route,             2) \
X(a, CALLBACK, REPEATED, MESSAGE,  task,              3)
#define SPT_Header_CALLBACK pb_default_field_callback
#define SPT_Header_DEFAULT NULL
#define SPT_Header_route_MSGTYPE SPT_Route
#define SPT_Header_task_MSGTYPE SPT_Task

extern const pb_msgdesc_t SPT_Route_msg;
extern const pb_msgdesc_t SPT_Task_msg;
extern const pb_msgdesc_t SPT_Header_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define SPT_Route_fields &SPT_Route_msg
#define SPT_Task_fields &SPT_Task_msg
#define SPT_Header_fields &SPT_Header_msg

/* Maximum encoded size of messages (where known) */
/* SPT_Task_size depends on runtime parameters */
/* SPT_Header_size depends on runtime parameters */
#define SPT_Route_size                           59

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif