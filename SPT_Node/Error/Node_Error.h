/***********************************************************/
/* Node_Error.h                                            */
/* Error Handling + Logging macros and defines             */
/***********************************************************/

#pragma once

#include "log.c\\src\\log.h"
#include <stdint.h> /* uint */

/* Errors */
#define ERRORCODE_SUCCESS   0
#define ERRORCODE_FAILED    1
#define ERRORCODE_UNKNOWN   2
#define ERRORCODE_GENERIC   3
#define ERRORCODE_INVALID   4
#define ERRORCODE_ALLOCATE  5
#define ERRORCODE_INPUT     6
#define ERRORCODE_NULL      7
#define ERRORCODE_WSA       8
#define ERRORCODE_SOCKET    9

#define ERRORCOUNT 10

typedef uint16_t errorcode;
extern errorcode g_error;
extern const char* ERRORSTR[ERRORCOUNT];

/* Main loop */
extern bool g_loop;

#define NUMARGS(...) sizeof(int[]){__VA_ARGS__}) / sizeof(int)

#define JMP_PRINT_IF(con, err, jmp, str, ...) do { if (con) { g_error = err; fprintf(stderr, str, ##__VA_ARGS__); goto jmp; } } while(0);
#define JMP_IF(con, err, jmp) do { if (con) { g_error = err; goto jmp; } } while(0);

#define RET_PRINT_IF(con, err, ret, str, ...) do { if (con) {  g_error = err; fprintf(stderr, str, ##__VA_ARGS__); return ret; } } while(0);
#define RET_IF(con, err, ret) do { if (con) {  g_error = err; return ret; } } while(0);

errorcode example_log(void);
errorcode init_error(int loglevel, int folderdepth);
