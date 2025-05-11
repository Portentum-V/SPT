
/* Node_Messages.h - Nano Protocol Buffers */

#pragma once

#include "../../SPT_Messages/message.pb.h"
#include "../../SPT_Messages/nanopb/pb.h"
#include "../../SPT_Messages/nanopb/pb_common.h"
#include "../../SPT_Messages/nanopb/pb_decode.h"
#include "../../SPT_Messages/nanopb/pb_encode.h"

// Need Socket for MSG_WAITALL and ULID stuff
#include "../Utilities/Node_Utilities.h"
#include "../Utilities/ulid-c\ulid.h"

pb_ostream_t pb_ostream_from_socket(int fd);
pb_istream_t pb_istream_from_socket(int fd);
