
/* Define Module and Status codes */

#include <stdint.h>

const enum MODULE {
	NONE      = 0,
	CONFIG    = 1 << 0,
	ERROR     = 1 << 1,
	MESSAGE   = 1 << 2,
	NETWORK   = 1 << 3,
	TRANSFORM = 1 << 4,
	UTILITY   = 1 << 5,
};