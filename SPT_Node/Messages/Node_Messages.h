
/* Node_Messages.h - Nano Protocol Buffers*/

#pragma once

#include "../../nanopb/pb.h"
#include "../../nanopb/pb_common.h"
#include "../../nanopb/pb_decode.h"
#include "../../nanopb/pb_encode.h"

// Need Socket for MSG_WAITALL...
#include "..\\Utilities\\Node_Utilities.h"

pb_ostream_t pb_ostream_from_socket(int fd);
pb_istream_t pb_istream_from_socket(int fd);
