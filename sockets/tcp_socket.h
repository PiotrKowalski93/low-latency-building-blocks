#pragma once

#include <functional>

#include "logging.h"
#include "socket_utils.h"

namespace Common {
    constexpr size_t TCPBufferSize = 64 * 1024 * 1024;

    struct TCPSocket {
        int fd_ = -1;

        
    };
}
