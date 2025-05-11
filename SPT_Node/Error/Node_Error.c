

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
	"WSA",
	"SOCKET",
	"REINIT"
};

errorcode init_error(int loglevel, int folderdepth) {
	log_trace("Log level: %d; folder depth: %d\n", loglevel, folderdepth);
	log_set_level(loglevel);
	log_set_depth(folderdepth);
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
