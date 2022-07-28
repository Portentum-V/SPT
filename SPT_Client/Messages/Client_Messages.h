
/* Client_Messages.h - Nano Protocol Buffers*/

#pragma once

#include "../../nanopb/pb.h"
#include "../../nanopb/pb_common.h"
#include "../../nanopb/pb_decode.h"
#include "../../nanopb/pb_encode.h"

// Hmmm, should figure out the best way to include things...
// Client_Utilities should probably hold pb_ostream_from_socket rather then this important it
#include "../Client_Utilities.h"

pb_ostream_t pb_ostream_from_socket(int fd);
pb_istream_t pb_istream_from_Socket(int fd);