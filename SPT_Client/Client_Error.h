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
#define ERRORCODE_INPUT     6

extern unsigned int errorcode;

#define NUMARGS(...) sizeof(int[]){__VA_ARGS__}) / sizeof(int)
// #define FAIL_IF_JMP(con, err, ...) if (con) { errorcode = err; fprintf(stderr, __VA_ARGS__); goto FAIL; }
#define IF_JMP(con, err, jmp, str, ...) do { if (con) { errorcode = err; fprintf(stderr, str, ##__VA_ARGS__); goto jmp; } } while(0);
// #define FAIL_IF_RET(con, ret, ...) if (con) { fprintf(stderr, __VA_ARGS__); return ret; }
#define IF_RET(con, err, ret, str, ...) do { if (con) {  errorcode = err; fprintf(stderr, str, ##__VA_ARGS__); return ret; } } while(0);