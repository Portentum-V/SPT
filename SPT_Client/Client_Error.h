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

#define IF_JMP(condition, error, jump, ...) if (condition) { errorcode = error; fprintf(stderr, __VA_ARGS__); goto jump; }
#define IF_RET(condition, error, retval, ...) if (condition) { errorcode = error; fprintf(stderr, __VA_ARGS__); return retval; }