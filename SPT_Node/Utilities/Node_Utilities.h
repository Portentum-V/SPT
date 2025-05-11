/***********************************************************/
/* Node_Utilities.h                                      */
/***********************************************************/

#pragma once

/* STD Headers */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

/* External Headers */
#include "ulid-c/ulid.h"
#include "cpuid.h"

/* SPT Headers */
#include "../Error/Node_Error.h"

#include "Node_OS.h"
#include "Node_Converters.h"
#include "Node_Validators.h"


/* Constants */
#define HWADDRSIZE 13
#define MAXADDRSIZE 40
#define UUID_SIZE 38
#define UUID_NONE "00000000-0000-0000-0000-000000000000"

#define ID_SIZE 16 /* Binary ULID/UUID */

#define BUFFER 256


/* ULID things */
extern struct ulid_generator* g_ulid_gen;
int init_ulid(void);

/* Functions */
void sleep_ms(int milliseconds);

void get_hostinfo(void);
