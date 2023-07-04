

#include "Node_Error.h"

errorcode g_error = ERRORCODE_UNKNOWN;
bool g_loop = true;

const char* ERRORSTR[ERRORCOUNT] = {
	"SUCCESS",
	"FAILED",
	"UNKNOWN",
	"GENERIC",
	"INVALID",
	"ALLOCATE",
	"INPUT",
	"NULL",
	"SOCKET"
};

errorcode init_error(int loglevel) {
	log_set_level(loglevel);
	example_log();
	return ERRORCODE_SUCCESS;
}

errorcode example_log(void) {
	log_trace("Trace");
	log_debug("Debug");
	log_info("Info");
	log_warn("Warn");
	log_error("Error");
	log_fatal("Fatal");
	return ERRORCODE_SUCCESS;
}
