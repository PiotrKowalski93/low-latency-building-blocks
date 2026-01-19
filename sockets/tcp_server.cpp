#pragma once

#include "tcp_server.h"

namespace Common
{
    auto TCPServer::destroy() {
        close(efd_);
        efd_ = -1;
        listener_socket_.destory();
    }
    
    
}
