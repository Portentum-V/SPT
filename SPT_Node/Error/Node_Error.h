/***********************************************************/
/* Node_Error.h                                          */
/* Error Handling macros and defines                       */
/***********************************************************/

#pragma once
#define ERRORCODE_FAILED   -1
#define ERRORCODE_SUCCESS	0
#define ERRORCODE_UNKNOWN	1
#define ERRORCODE_GENERIC   2
#define ERRORCODE_ALLOCATE  4
#define ERRORCODE_SOCKET    5
#define ERRORCODE_INPUT     6

extern const char* ERRORSTR[7];

extern int ERRORCODE;

#include "..\\..\\log.c\\src\\log.h"

#define NUMARGS(...) sizeof(int[]){__VA_ARGS__}) / sizeof(int)

#define JMP_IF(con, err, jmp, str, ...) do { if (con) { ERRORCODE = err; fprintf(stderr, str, ##__VA_ARGS__); goto jmp; } } while(0);

#define RET_IF(con, err, ret, str, ...) do { if (con) {  ERRORCODE = err; fprintf(stderr, str, ##__VA_ARGS__); return ret; } } while(0);