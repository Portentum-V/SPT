/***********************************************************/
/* Client_Error.h                                          */
/* Error Handling macros and defines                       */
/***********************************************************/

#pragma once
#define ERRORCODE_SUCCESS	0
#define ERRORCODE_UNKNOWN	1
#define ERRORCODE_GENERIC   2
#define ERRORCODE_FAILED	3
#define ERRORCODE_ALLOCATE  4
#define ERRORCODE_SOCKET    5

extern unsigned int errorcode;

#define NUMARGS(...) sizeof(int[]){__VA_ARGS__}) / sizeof(int)
// #define FAIL_IF_JMP(con, err, ...) if (con) { errorcode = err; fprintf(stderr, __VA_ARGS__); goto FAIL; }
#define FAIL_IF_JMP(con, str, ...) do { if (con) { fprintf(stderr, str, ##__VA_ARGS__); goto FAIL; } } while(0);
// #define FAIL_IF_RET(con, ret, ...) if (con) { fprintf(stderr, __VA_ARGS__); return ret; }
#define FAIL_IF_RET(con, ret, str, ...) do { if (con) { fprintf(stderr, str, ##__VA_ARGS__); return ret; } } while(0);
