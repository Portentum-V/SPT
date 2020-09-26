/***********************************************************/
/* Client_Utilities.h                                      */
/***********************************************************/

#pragma once
int check_port(char* input);
int menu(int argc, char* argv[], int (*client)(char*, char*, int));