/***********************************************************/
/* Client_Error.h                                          */
/* Error Handling macros and defines                       */
/***********************************************************/

#pragma once
#define ERRORCODE_SUCCESS	0
#define ERRORCODE_UNKNOWN	1
#define ERRORCODE_GENERIC   2
#define ERRORCODE_FAILED	3

#define FAIL_IF(condition, error) if (condition) {errcode = error; goto FAIL;}
#define FAIL_IF_PRT(condition, string, error) if (condition) {errocode = error; fprintf(stderr, string); goto FAIL;}