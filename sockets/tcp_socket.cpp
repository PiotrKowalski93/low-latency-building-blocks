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
        if(len > 0){
            memcpy(send_buffer_ + next_send_valid_index_, data, len);
            next_send_valid_index_ += len;
        }
    }

    auto TCPSocket::sendAndRecv() noexcept -> bool { 

        // Buffor for ancillary data - 
        char ctrl[CMSG_SPACE(sizeof(struct timeval))];
        struct cmsghdr *cmsg = (struct cmsghdr *) &ctrl;

        // I/O vector. Struct that contains pointer to start and length
        // we use it as a buffor for data to send
        struct iovec iov;
        iov.iov_base = rcv_buffer_ + next_rcv_valid_index_;
        iov.iov_len = TCPBufferSize - next_rcv_valid_index_;

        msghdr msg;
        msg.msg_control = ctrl;
        msg.msg_controllen = sizeof(ctrl);
        msg.msg_name = &inInAddr;
        msg.msg_namelen = sizeof(inInAddr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        // recvmsg - writes to many places at once
        // msg.msg_iov -> buffor for incoming message
        // msg.msg_name -> place for sender info
        // msg.msg_control -> timestamp added by kernel
        const auto n_rcv = recvmsg(fd_, &msg, MSG_DONTWAIT);
        if(n_rcv > 0) {
            //TODO:
        }

    }
}
