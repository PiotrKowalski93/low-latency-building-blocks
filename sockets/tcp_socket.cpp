#pragma once

#include "logging.h"
#include "tcp_socket.h"

namespace Common {
    auto TCPSocket::destory() noexcept -> void {
        close(fd_);
        fd_ = -1;
    };

    auto TCPSocket::connect(const std::string &ip, const std::string &iface, int port, bool is_listening) -> int {
        destory();
        fd_ = createSocket(logger_, ip, iface, port, false, false, is_listening, 0, true);
        
        // Set local info in TCPSocket obj
        // Bind to all local addresses
        inInAddr.sin_addr.s_addr = INADDR_ANY;
        // set porty
        inInAddr.sin_port = htons(port);
        //AF_INET = IPv4
        inInAddr.sin_family = AF_INET;

        return fd_;
    };

    auto TCPSocket::send(const void *data, size_t len) noexcept -> void {
        
    }

}
