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

#define FAIL_IF_JMP(condition, error, ...) if (condition) { errorcode = error; fprintf(stderr, __VA_ARGS__); goto FAIL; }
#define FAIL_IF_RET(condition, ret, ...) if (condition) { fprintf(stderr, __VA_ARGS__); return retVal; }